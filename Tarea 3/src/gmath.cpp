# include "gmath.hpp"

double vec_norm (double * vec, uint64_t n) {
    double result = 0;
    for (uint64_t i = 0; i < n; i ++)
        result += vec [i] * vec [i];
    return sqrt (result);
}

void vec_normalize (double * vec, uint64_t n) {
    double norm = vec_norm (vec, n);
    for (uint64_t i = 0; i < n; i ++)
        vec [i] /= norm;
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

void scalar_vec_mul (double scalar, double * vec, uint64_t n) {
    for (uint64_t i = 0; i < n; i ++)
        vec [i] *= scalar;
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

void mat_identity (double * mat, uint64_t n) {
    double (* mat_) [n] = reinterpret_cast <double (*) [n]> (mat);
    for (uint64_t i = 0; i < n; i ++) {
        mat_ [i] [i] = 1;
        for (uint64_t j = i + 1; j < n; j ++) {
            mat_ [i] [j] = 0;
            mat_ [j] [i] = 0;
        }
    }
}

void mat_transpose (double * mat_a, double * mat_b, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_1] = reinterpret_cast <double (*) [n_1]> (mat_b);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_b_ [j] [i] = mat_a_ [i] [j];
    }
}

void mat_add (double * mat_a, double * mat_b, double * mat_c, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    double (* mat_c_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_c);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_c_ [i] [j] = mat_a_ [i] [j] + mat_b_ [i] [j];
    }
}

void mat_sub (double * mat_a, double * mat_b, double * mat_c, uint64_t n_1, uint64_t n_2) {
    double (* mat_a_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_a);
    double (* mat_b_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_b);
    double (* mat_c_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat_c);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_c_ [i] [j] = mat_a_ [i] [j] - mat_b_ [i] [j];
    }
}

void scalar_mat_mul (double scalar, double * mat, uint64_t n_1, uint64_t n_2) {
    double (* mat_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat);
    for (uint64_t i = 0; i < n_1; i ++) {
        for (uint64_t j = 0; j < n_2; j ++)
            mat_ [i] [j] *= scalar;
    }
}

void mat_vec_mul (double * mat, double * vec_a, double * vec_b, uint64_t n_1, uint64_t n_2) {
    double (* mat_) [n_2] = reinterpret_cast <double (*) [n_2]> (mat);
    for (uint64_t i = 0; i < n_1; i ++) {
        vec_b [i] = 0;
        for (uint64_t j = 0; j < n_2; j ++)
            vec_b[i] += mat_ [i] [j] * vec_a [j];
    }
}

void mat_mul (double * mat_a, double * mat_b, double * mat_c, uint64_t n_1, uint64_t n_2, uint64_t n_3) {
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

void camera_look_at_mat4 (double * position, double * target, double * ref, double * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    vec_sub (target, position, mat_ [1], 3);
    vec_normalize (mat_ [1], 3);
    vec3_cross_prod (mat_ [1], ref, mat_ [0]);
    vec_normalize (mat_ [0], 3);
    vec3_cross_prod (mat_ [0], mat_ [1], mat_ [2]);
    mat_ [0] [3] = - vec_dot_prod (position, mat_ [0], 3);
    mat_ [1] [3] = - vec_dot_prod (position, mat_ [1], 3);
    mat_ [2] [3] = - vec_dot_prod (position, mat_ [2], 3);
    mat_ [3] [0] = 0;
    mat_ [3] [1] = 0;
    mat_ [3] [2] = 0;
    mat_ [3] [3] = 1;
}

void orthogonal_proj_mat4 (double aspect_ratio, double ext_v, double near, double far, double * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    memset (mat, 0, 16 * sizeof (double));
    mat_ [2] [2] = 2 / ext_v;
    mat_ [0] [0] = mat_ [2] [2] / aspect_ratio;
    mat_ [1] [1] = 1 / (far - near);
    mat_ [1] [3] = - near * mat_ [1] [1];
    mat_ [3] [3] = 1;
}

void perspective_proj_mat4 (double aspect_ratio, double fov_v, double near, double far, double * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    memset (mat, 0, 16 * sizeof (double));
    mat_ [2] [2] = 1 / tan (fov_v * .5);
    mat_ [0] [0] = mat_ [2] [2] / aspect_ratio;
    mat_ [1] [1] = far / (far - near);
    mat_ [1] [3] = - near * mat_ [1] [1];
    mat_ [3] [1] = 1;
}

void transform_euler_zyx_mat4 (double * position, double * rotation, double * scale, double * mat) {
    double (* mat_) [4] = reinterpret_cast <double (*) [4]> (mat);
    double cos_values [3] = { cos (rotation [0]), cos (rotation [1]), cos (rotation [2]) };
    double sin_values [3] = { sin (rotation [0]), sin (rotation [1]), sin (rotation [2]) };
    mat_ [0] [0] = scale [0] * cos_values [1] * cos_values [2];
    mat_ [0] [1] = - scale [1] * cos_values [1] * sin_values [2];
    mat_ [0] [2] = scale [2] * sin_values [1];
    mat_ [0] [3] = position [0];
    mat_ [1] [0] = scale [0] * (cos_values [0] * sin_values [2] + cos_values [2] * sin_values [0] * sin_values [1]);
    mat_ [1] [1] = scale [1] * (cos_values [0] * cos_values [2] - sin_values [0] * sin_values [1] * sin_values [2]);
    mat_ [1] [2] = - scale [2] * cos_values [1] * sin_values [0];
    mat_ [1] [3] = position [1];
    mat_ [2] [0] = scale [0] * (sin_values [0] * sin_values [2] - cos_values [0] * cos_values [2] * sin_values [1]);
    mat_ [2] [1] = scale [1] * (cos_values [2] * sin_values [0] + cos_values [0] * sin_values [1] * sin_values [2]);
    mat_ [2] [2] = scale [2] * cos_values [0] * cos_values [1];
    mat_ [2] [3] = position [2];
    mat_ [3] [0] = 0;
    mat_ [3] [1] = 0;
    mat_ [3] [2] = 0;
    mat_ [3] [3] = 1;
}

void mat4_chain_mul (double * * array, uint64_t n, double * mat) {
    double temp [16];
    mat_identity (mat, 4);
    for (uint64_t i = 0; i < n; i ++) {
        memcpy (temp, mat, 16 * sizeof (double));
        mat_mul (temp, array [i], mat, 4, 4, 4);
    }
}