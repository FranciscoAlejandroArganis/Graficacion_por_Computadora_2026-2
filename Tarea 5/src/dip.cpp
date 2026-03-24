# include "dip.hpp"

uint8_t to_color_component (double value) {
    int64_t component = round (value);
    if (component < 0)
        return 0;
    if (component > 255)
        return 255;
    return component;
}

uint32_t apply_kernel (Image image, Rect window, Kernel kernel, Coord center) {
    uint32_t (* image_data) [image.extent.width] = reinterpret_cast <uint32_t (*) [image.extent.width]> (image.data);
    uint64_t kernel_size = (kernel.radius << 1) + 1;
    double (* kernel_data) [kernel_size] = reinterpret_cast <double (*) [kernel_size]> (kernel.data);
    double sums [3] = {};
    uint64_t y = window.start.y;
    while (y <= window.end.y) {
        uint64_t x = window.start.x;
        while (x <= window.end.x) {
            sums [0] += qRed (image_data [y] [x]) * kernel_data [y - center.y + kernel.radius] [x - center.x + kernel.radius];
            sums [1] += qGreen (image_data [y] [x]) * kernel_data [y - center.y + kernel.radius] [x - center.x + kernel.radius];
            sums [2] += qBlue (image_data [y] [x]) * kernel_data [y - center.y + kernel.radius] [x - center.x + kernel.radius];
            x ++;
        }
        y ++;
    }
    return qRgba (
        to_color_component (sums [0]),
        to_color_component (sums [1]),
        to_color_component (sums [2]),
        qAlpha (image_data [center.y] [center.x])
    );
}

Rect build_kernel_window (Rect rect, Coord center, uint64_t radius) {
    Rect window;
    if (center.x >= rect.start.x + radius)
        window.start.x = center.x - radius;
    else
        window.start.x = rect.start.x;
    if (center.y >= rect.start.y + radius)
        window.start.y = center.y - radius;
    else
        window.start.y = rect.start.y;
    if (center.x + radius <= rect.end.x)
        window.end.x = center.x + radius;
    else
        window.end.x = rect.end.x;
    if (center.y + radius <= rect.end.y)
        window.end.y = center.y + radius;
    else
        window.end.y = rect.end.y;
    return window;
}

void convolution_filter_h (Image image, Rect rect, Kernel kernel, uint32_t * queue) {
    uint32_t (* image_data) [image.extent.width] = reinterpret_cast <uint32_t (*) [image.extent.width]> (image.data);
    uint64_t capacity = kernel.radius * (1 + image.extent.width);
    uint64_t size = 0;
    uint64_t head = 0;
    Coord write = { .x = 0, .y = 0 };
    Coord center;
    center.y = rect.start.y;
    while (center.y <= rect.end.y) {
        center.x = rect.start.x;
        while (center.x <= rect.end.x) {
            Rect window = build_kernel_window (rect, center, kernel.radius);
            uint32_t value = apply_kernel (image, window, kernel, center);
            if (size < capacity) {
                queue [size] = value;
                size ++;
            } else {
                image_data [write.y] [write.x] = queue [head];
                write.x ++;
                if (write.x >= image.extent.width) {
                    write.x = 0;
                    write.y ++;
                }
                queue [head] = value;
                head ++;
                if (head >= capacity)
                    head = 0;
            }
            center.x ++;
        }
        center.y ++;
    }
    while (size > 0) {
        image_data [write.y] [write.x] = queue [head];
        write.x ++;
        if (write.x >= image.extent.width) {
            write.x = 0;
            write.y ++;
        }
        head ++;
        if (head >= capacity)
            head = 0;
        size --;
    }
}

void convolution_filter_v (Image image, Rect rect, Kernel kernel, uint32_t * queue) {
    uint32_t (* image_data) [image.extent.width] = reinterpret_cast <uint32_t (*) [image.extent.width]> (image.data);
    uint64_t capacity = kernel.radius * (1 + image.extent.height);
    uint64_t size = 0;
    uint64_t head = 0;
    Coord write = { .x = 0, .y = 0 };
    Coord center;
    center.x = rect.start.x;
    while (center.x <= rect.end.x) {
        center.y = rect.start.y;
        while (center.y <= rect.end.y) {
            Rect window = build_kernel_window (rect, center, kernel.radius);
            uint32_t value = apply_kernel (image, window, kernel, center);
            if (size < capacity) {
                queue [size] = value;
                size ++;
            } else {
                image_data [write.y] [write.x] = queue [head];
                write.y ++;
                if (write.y >= image.extent.height) {
                    write.y = 0;
                    write.x ++;
                }
                queue [head] = value;
                head ++;
                if (head >= capacity)
                    head = 0;
            }
            center.y ++;
        }
        center.x ++;
    }
    while (size > 0) {
        image_data [write.y] [write.x] = queue [head];
        write.y ++;
        if (write.y >= image.extent.height) {
            write.y = 0;
            write.x ++;
        }
        head ++;
        if (head >= capacity)
            head = 0;
        size --;
    }
}

void convolution_filter (Image image, Rect rect, Kernel kernel, uint32_t * queue) {
    if (image.extent.width <= image.extent.height)
        convolution_filter_h (image, rect, kernel, queue);
    else
        convolution_filter_v (image, rect, kernel, queue);
}
