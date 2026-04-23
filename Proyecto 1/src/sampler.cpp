# include "sampler.hpp"

void Sampler::raw_sample_nearest (double * position, double * sample) {
    uint8_t (* data) [image.extent.width] [image.channels] = reinterpret_cast <uint8_t (*) [image.extent.width] [image.channels]> (image.data);
    Coord coord;
    coord.x = round (position [0]);
    coord.y = round (position [1]);
    uint64_t channel = 0;
    for (uint64_t channel = 0; channel < image.channels; channel ++)
        sample [channel] = data [coord.y] [coord.x] [channel];
}

void Sampler::raw_sample_bilinear (double * position, double * sample) {
    uint8_t (* data) [image.extent.width] [image.channels] = reinterpret_cast <uint8_t (*) [image.extent.width] [image.channels]> (image.data);
    Rect rect;
    rect.start.x = floor (position [0]);
    rect.start.y = floor (position [1]);
    rect.end.x = ceil (position [0]);
    rect.end.y = ceil (position [1]);
    if (rect.start.x == rect.end.x) {
        if (rect.start.y == rect.end.y) {
            for (uint64_t channel = 0; channel < image.channels; channel ++)
                sample [channel] = data [rect.start.y] [rect.start.x] [channel];
        } else {
            double inputs [2];
            inputs [0] = rect.start.y;
            inputs [1] = rect.end.y;
            for (uint64_t channel = 0; channel < image.channels; channel ++) {
                double outputs [2];
                outputs [0] = data [rect.start.y] [rect.start.x] [channel];
                outputs [1] = data [rect.end.y] [rect.start.x] [channel];
                sample [channel] = multilinear_interpolation (1, position + 1, & inputs, outputs);
            }
        }
    } else if (rect.start.y == rect.end.y) {
        double inputs [2];
        inputs [0] = rect.start.x;
        inputs [1] = rect.end.x;
        for (uint64_t channel = 0; channel < image.channels; channel ++) {
            double outputs [2];
            outputs [0] = data [rect.start.y] [rect.start.x] [channel];
            outputs [1] = data [rect.start.y] [rect.end.x] [channel];
            sample [channel] = multilinear_interpolation (1, position, & inputs, outputs);
        }
    } else {
        double inputs [2] [2];
        inputs [0] [0] = rect.start.x;
        inputs [0] [1] = rect.end.x;
        inputs [1] [0] = rect.start.y;
        inputs [1] [1] = rect.end.y;
        for (uint64_t channel = 0; channel < image.channels; channel ++) {
            double outputs [4];
            outputs [0] = data [rect.start.y] [rect.start.x] [channel];
            outputs [1] = data [rect.start.y] [rect.end.x] [channel];
            outputs [2] = data [rect.end.y] [rect.start.x] [channel];
            outputs [3] = data [rect.end.y] [rect.end.x] [channel];
            sample [channel] = multilinear_interpolation (2, position, inputs, outputs);
        }
    }
}

void Sampler::sample (double * position, double * sample) {
    switch (address_mode) {
        case SAMPLER_ADDRESS_MODE_CLAMP :
            position [0] = clamp (position [0], 0, 1);
            position [1] = clamp (position [1], 0, 1);
            break;
        case SAMPLER_ADDRESS_MODE_REPEAT :
            position [0] = frac_part (position [0]);
            position [1] = frac_part (position [1]);
    }
    position [0] *= image.extent.width - 1;
    position [1] *= image.extent.height - 1;
    switch (filter) {
        case SAMPLER_FILTER_NEAREST :
            raw_sample_nearest (position, sample);
            break;
        case SAMPLER_FILTER_LINEAR :
            raw_sample_bilinear (position, sample);
    }
    vec_scalar_mul (sample, 1. / UINT8_MAX, sample, image.channels);
    if (image.channels > 2)
        std::swap (sample [0], sample [2]);
}

void SamplerCube::sample (double * direction, double * sample) {
    SamplerCubeFace face;
    double t;
    double position [2];
    double min = INFINITY;
    if (direction [0] < 0) {
        t = - 1 / direction [0];
        min = t;
        face = SAMPLER_CUBE_FACE_LEFT;
        position [0] = t * direction [1];
        position [1] = - t * direction [2];
    } else if (direction [0] > 0) {
        t = 1 / direction [0];
        min = t;
        face = SAMPLER_CUBE_FACE_RIGHT;
        position [0] = - t * direction [1];
        position [1] = - t * direction [2];
    }
    if (direction [1] < 0) {
        t = - 1 / direction [1];
        if (t < min) {
            min = t;
            face = SAMPLER_CUBE_FACE_BACK;
            position [0] = - t * direction [0];
            position [1] = - t * direction [2];
        }
    } else if (direction [1] > 0) {
        t = 1 / direction [1];
        if (t < min) {
            min = t;
            face = SAMPLER_CUBE_FACE_FRONT;
            position [0] = t * direction [0];
            position [1] = - t * direction [2];
        }
    }
    if (direction [2] < 0) {
        t = - 1 / direction [2];
        if (t < min) {
            min = t;
            face = SAMPLER_CUBE_FACE_DOWN;
            position [0] = t * direction [0];
            position [1] = - t * direction [1];
        }
    } else if (direction [2] > 0) {
        t = 1 / direction [2];
        if (t < min) {
            min = t;
            face = SAMPLER_CUBE_FACE_UP;
            position [0] = t * direction [0];
            position [1] = t * direction [1];
        }
    }
    position [0] = .5 * (position [0] + 1);
    position [1] = .5 * (position [1] + 1);
    Sampler sampler = {
        .image = cube_map [face],
        .filter = filter
    };
    sampler.sample (position, sample);
}