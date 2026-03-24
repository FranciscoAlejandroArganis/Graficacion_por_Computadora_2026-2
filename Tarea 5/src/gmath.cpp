# include "gmath.hpp"

double clamp (double x, double a, double b) {
    if (x < a)
        return a;
    if (x > b)
        return b;
    return x;
}

double lerp (double t, double a, double b) {
    return a * (1 - t) + b * t;
}

double dlerp (double t, double a, double b) {
    return .5 * (a * (1 - t) + b * (t + 1));
}

double blerp (double u, double v, double a, double b, double c) {
    return a * (1 - u - v) + b * u + c * v;
}

double vec_norm (double * vec, uint64_t n) {
    double result = 0;
    for (uint64_t i = 0; i < n; i ++)
        result += vec [i] * vec [i];
    return sqrt (result);
}

void vec_unit (double * vec_a, double * vec_b, uint64_t n) {
    double factor  = 1 / vec_norm (vec_a, n);
    for (uint64_t i = 0; i < n; i ++)
        vec_b [i] = vec_a [i] * factor;
}

void vec_to_homogeneous (double * vec_a, double * vec_b, uint64_t n) {
    memcpy (vec_b, vec_a, n * sizeof (double));
    vec_b [n] = 1;
}

void vec_from_homogeneous (double * vec_a, double * vec_b, uint64_t n) {
    n --;
    double factor = 1. / vec_a [n];
    for (uint64_t i = 0; i < n; i ++)
        vec_b [i] = vec_a [i] * factor;
}

void vec_add (double * vec_a, double * vec_b, double * vec_c, uint64_t n) {
    for (uint64_t i = 0; i < n; i ++)
        vec_c [i] = vec_a [i] + vec_b [i];
}

void vec_sub (double * vec_a, double * vec_b, double * vec_c, uint64_t n) {
    for (uint64_t i = 0; i < n; i ++)
        vec_c [i] = vec_a [i] - vec_b [i];
}

void vec_emul (double * vec_a, double * vec_b, double * vec_c, uint64_t n) {
    for (uint64_t i = 0; i < n; i ++)
        vec_c [i] = vec_a [i] * vec_b [i];
}

void vec_scalar_mul (double * vec_a, double scalar, double * vec_b, uint64_t n) {
    for (uint64_t i = 0; i < n; i ++)
        vec_b [i] = vec_a [i] * scalar;
}

double vec_dot_prod (double * vec_a, double * vec_b, uint64_t n) {
    double result = 0;
    for (uint64_t i = 0; i < n; i ++)
        result += vec_a [i] * vec_b [i];
    return result;
}

void vec3_cross_prod (double * vec_a, double * vec_b, double * vec_c) {
    vec_c [0] = vec_a [1] * vec_b [2] - vec_a [2] * vec_b [1];
    vec_c [1] = vec_a [2] * vec_b [0] - vec_a [0] * vec_b [2];
    vec_c [2] = vec_a [0] * vec_b [1] - vec_a [1] * vec_b [0];
}

void vec_reflect (double * incident, double * normal, double * reflected, uint64_t n) {
    vec_scalar_mul (normal, - 2 * vec_dot_prod (normal, incident, n), reflected, n);
    vec_add (incident, reflected, reflected, n);
}

void mat_identity (void * mat, uint64_t n) {
    double (* mat_) [n] = reinterpret_cast <double (*) [n]> (mat);
    for (uint64_t i = 0; i < n; i ++) {
        mat_ [i] [i] = 1;
        for (uint64_t j = i + 1; j < n; j ++) {
            mat_ [i] [j] = 0;
            mat_ [j] [i] = 0;
        }
    }
}

void mat_transpose (void * mat_a, void * mat_b, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_1] = reinterpret_cast <double (*) [n_1]> (mat_b);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_b_ [j] [i] = mat_a_ [i] [j];
    }
}

void mat_add (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    double (* mat_c_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_c);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_c_ [i] [j] = mat_a_ [i] [j] + mat_b_ [i] [j];
    }
}

void mat_sub (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    double (* mat_c_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_c);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_c_ [i] [j] = mat_a_ [i] [j] - mat_b_ [i] [j];
    }
}

void mat_emul (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    double (* mat_c_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_c);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_c_ [i] [j] = mat_a_ [i] [j] * mat_b_ [i] [j];
    }
}

void mat_scalar_mul (void * mat_a, double scalar, void * mat_b, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_b_ [i] [j] = mat_a_ [i] [j] * scalar;
    }
}

void mat_vec_mul (void * mat, double * vec_a, double * vec_b, uint64_t n_1, uint64_t n_2) {
    double (* mat_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat);
    for (uint64_t i = 0; i < n_1; i ++) {
        vec_b [i] = 0;
        for (uint64_t j = 0; j < n_2; j ++)
            vec_b [i] += mat_ [i] [j] * vec_a [j];
    }
}

void mat_mul (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2, uint64_t n_3) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_3] = reinterpret_cast <double (*) [n_3]> (mat_b);
    double (* mat_c_) [n_3] = reinterpret_cast <double (*) [n_3]> (mat_c);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_3; j ++) {
            mat_c_ [i] [j] = 0;
            for (uint64_t k = 0; k < n_2; k ++)
                mat_c_ [i] [j] += mat_a_ [i] [k] * mat_b_ [k] [j];
        }
    }
}

double mat_inverse (void * mat_a, void * mat_b, uint64_t n) {
    double (* mat_a_) [n] = reinterpret_cast <double (*) [n]> (mat_a);
    double (* mat_b_) [n] = reinterpret_cast <double (*) [n]> (mat_b);
    double determinant = 1;
    mat_identity (mat_b, n);
    for (uint64_t j = 0; j < n; j ++) {
        uint64_t i = j;
        while (i < n && mat_a_ [i] [j] == 0)
            i ++;
        if (i == n)
            return 0;
        if (i > j) {
            vec_add (mat_a_ [j], mat_a_ [i], mat_a_ [j], n);
            vec_add (mat_b_ [j], mat_b_ [i], mat_b_ [j], n);
        }
        determinant *= mat_a_ [j] [j];
        double alpha = 1 / mat_a_ [j] [j];
        vec_scalar_mul (mat_a_ [j], alpha, mat_a_ [j], n);
        vec_scalar_mul (mat_b_ [j], alpha, mat_b_ [j], n);
        for (i = 0; i < j; i ++) {
            alpha = - mat_a_ [i] [j];
            for (uint64_t k = 0; k < n; k ++) {
                mat_a_ [i] [k] += alpha * mat_a_ [j] [k];
                mat_b_ [i] [k] += alpha * mat_b_ [j] [k];
            }
        }
        for (i = j + 1; i < n; i ++) {
            alpha = - mat_a_ [i] [j];
            for (uint64_t k = 0; k < n; k ++) {
                mat_a_ [i] [k] += alpha * mat_a_ [j] [k];
                mat_b_ [i] [k] += alpha * mat_b_ [j] [k];
            }
        }
    }
    return determinant;
}

void camera_look_at_mat4 (double * position, double * target, double * ref, void * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    vec_sub (target, position, mat_ [1], 3);
    vec_unit (mat_ [1], mat_ [1], 3);
    vec3_cross_prod (mat_ [1], ref, mat_ [0]);
    vec_unit (mat_ [0], mat_ [0], 3);
    vec3_cross_prod (mat_ [0], mat_ [1], mat_ [2]);
    mat_ [0] [3] = - vec_dot_prod (position, mat_ [0], 3);
    mat_ [1] [3] = - vec_dot_prod (position, mat_ [1], 3);
    mat_ [2] [3] = - vec_dot_prod (position, mat_ [2], 3);
    mat_ [3] [0] = 0;
    mat_ [3] [1] = 0;
    mat_ [3] [2] = 0;
    mat_ [3] [3] = 1;
}

void orthogonal_proj_mat4 (double aspect_ratio, double ext_v, double near, double far, void * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    memset (mat, 0, 16 * sizeof (double));
    mat_ [2] [2] = 2 / ext_v;
    mat_ [0] [0] = mat_ [2] [2] / aspect_ratio;
    mat_ [1] [1] = 1 / (far - near);
    mat_ [1] [3] = - near * mat_ [1] [1];
    mat_ [3] [3] = 1;
}

void perspective_proj_mat4 (double aspect_ratio, double fov_v, double near, double far, void * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    memset (mat, 0, 16 * sizeof (double));
    mat_ [2] [2] = 1 / tan (fov_v * .5);
    mat_ [0] [0] = mat_ [2] [2] / aspect_ratio;
    mat_ [1] [1] = far / (far - near);
    mat_ [1] [3] = - near * mat_ [1] [1];
    mat_ [3] [1] = 1;
}

void transform_euler_zyx_mat4 (double * position, double * rotation, double * scale, void * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    double cos_values [3] = { cos (rotation [0]), cos (rotation [1]), cos (rotation [2]) };
    double sin_values [3] = { sin (rotation [0]), sin (rotation [1]), sin (rotation [2]) };
    mat_ [0] [0] = scale [0] * cos_values [1] * cos_values [2];
    mat_ [0] [1] = scale [1] * (sin_values [0] * sin_values [1] * cos_values [2] - cos_values [0] * sin_values [2]);
    mat_ [0] [2] = scale [2] * (cos_values [0] * sin_values [1] * cos_values [2] + sin_values [0] * sin_values [2]);
    mat_ [0] [3] = position [0];
    mat_ [1] [0] = scale [0] * cos_values [1] * sin_values [2];
    mat_ [1] [1] = scale [1] * (sin_values [0] * sin_values [1] * sin_values [2] + cos_values [0] * cos_values [2]);
    mat_ [1] [2] = scale [2] * (cos_values [0] * sin_values [1] * sin_values [2] - sin_values [0] * cos_values [2]);
    mat_ [1] [3] = position [1];
    mat_ [2] [0] = - scale [0] * sin_values [1];
    mat_ [2] [1] = scale [1] * sin_values [0] * cos_values [1];
    mat_ [2] [2] = scale [2] * cos_values [0] * cos_values [1];
    mat_ [2] [3] = position [2];
    mat_ [3] [0] = 0;
    mat_ [3] [1] = 0;
    mat_ [3] [2] = 0;
    mat_ [3] [3] = 1;
}

void mat4_chain_mul (void * * array, uint64_t n, void * mat) {
    double temp [4] [4];
    mat_identity (mat, 4);
    for (uint64_t i = 0; i < n; i ++) {
        memcpy (temp, mat, 16 * sizeof (double));
        mat_mul (temp, array [i], mat, 4, 4, 4);
    }
}

void bound_points (void * mat, void * aabb, uint64_t n_1, uint64_t n_2) {
    double (* mat_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat);
    double (* aabb_) [n_2] = reinterpret_cast <double (*) [n_2]> (aabb);
    for (uint64_t j = 0; j < n_2; j ++) {
        aabb_ [0] [j] = mat_ [0] [j];
        aabb_ [1] [j] = mat_ [0] [j];
        for (uint64_t i = 1; i < n_1; i ++) {
            if (mat_ [i] [j] < aabb_ [0] [j]) {
                aabb_ [0] [j] = mat_ [i] [j];
            } else if (mat_ [i] [j] > aabb_ [1] [j]) {
                aabb_ [1] [j] = mat_ [i] [j];
            }
        }
    }
}

void linear_param (void * mat_a, void * mat_b, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    memcpy (mat_b, mat_a, n_2 * sizeof (double));
    for (uint64_t i = 1; i < n_1; i ++)
        vec_sub (mat_a_ [i], mat_a_ [0], mat_b_ [i], n_2);
}

bool aabb_empty (void * aabb, uint64_t n) {
    double (* aabb_) [n] = reinterpret_cast <double (*) [n]> (aabb);
    for (uint64_t i = 0; i < n; i ++) {
        if (aabb_ [0] [i] > aabb_ [1] [i])
            return true;
    }
    return false;
}

void aabb_intersection (void * aabb_a, void * aabb_b, void * aabb_c, uint64_t n) {
    double (* aabb_a_) [n] = reinterpret_cast <double (*) [n]> (aabb_a);
    double (* aabb_b_) [n] = reinterpret_cast <double (*) [n]> (aabb_b);
    double (* aabb_c_) [n] = reinterpret_cast <double (*) [n]> (aabb_c);
    for (uint64_t i = 0; i < n; i ++) {
        if (aabb_a_ [0] [i] <= aabb_b_ [0] [i])
            aabb_c_ [0] [i] = aabb_b_ [0] [i];
        else
            aabb_c_ [0] [i] = aabb_a_ [0] [i];
        if (aabb_a_ [1] [i] <= aabb_b_ [1] [i])
            aabb_c_ [1] [i] = aabb_a_ [1] [i];
        else
            aabb_c_ [1] [i] = aabb_b_ [1] [i];
    }
}

void line_normal (double * point_a, double * point_b, double * normal) {
    normal [0] = point_a [1] - point_b [1];
    normal [1] = - point_a [0] + point_b [0];
}

void plane_normal (double * point_a, double * point_b, double * point_c, double * normal) {
    normal [0] = point_a [1] * (point_b [2] - point_c [2]) - point_a [2] * (point_b [1] - point_c [1]) + point_b [1] * point_c [2] - point_c [1] * point_b [2];
    normal [1] = - point_a [0] * (point_b [2] - point_c [2]) + point_a [2] * (point_b [0] - point_c [0]) - point_b [0] * point_c [2] + point_c [0] * point_b [2];
    normal [2] = point_a [0] * (point_b [1] - point_c [1]) - point_a [1] * (point_b [0] - point_c [0]) + point_b [0] * point_c [1] - point_c [0] * point_b [1];
}
