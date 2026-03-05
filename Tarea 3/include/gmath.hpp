# pragma once

# include <cstdint>
# include <cstring>
# include <cmath>

/// @brief Calcula la norma de un vector
/// @param vec es el vector
/// @param n es la dimensión de `vec`
/// @return la norma L2 de `vec`
double vec_norm (double * vec, uint64_t n);

/// @brief Normaliza un vector para que tenga norma 1
/// @param vec es el vector
/// @param n es la dimensión de `vec`
void vec_normalize (double * vec, uint64_t n);

/// @brief Convierte un vector en coordenales normales a coordenadas homogéneas
/// @param vec_a es el vector
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_to_homogeneous (double * vec_a, double * vec_b, uint64_t n);

/// @brief Convierte un vector en coordenadas homogeneas a coordenadas normales
/// @param vec_a es el vector en coordenadas homogéneas
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_from_homogeneous (double * vec_a, double * vec_b, uint64_t n);

/// @brief Suma dos vectores
/// @param vec_a es el primer vector
/// @param vec_b es el segundo vector
/// @param vec_c es donde se guarda el resultado
/// @param n es la dimensión de `vec_a` y `vec_b`
void vec_add (double * vec_a, double * vec_b, double * vec_c, uint64_t n);

/// @brief Resta dos vectores
/// @param vec_a es el primer vector
/// @param vec_b es el segundo vector
/// @param vec_c es donde se guarda el resultado
/// @param n es la dimensión de `vec_a` y `vec_b`
void vec_sub (double * vec_a, double * vec_b, double * vec_c, uint64_t n);

/// @brief Multiplica un escalar por un vector
/// @param scalar es el escalar
/// @param vec es el vector
/// @param n es la dimensión de `vec_b`
void scalar_vec_mul (double scalar, double * vec, uint64_t n);

/// @brief Calcula el producto punto entre vectores
/// @param vec_a es el primer vector
/// @param vec_b es el segundo vector
/// @param n es la dimensión `vec_a` y `vec_b`
/// @return el producto punto de ambos vectores
double vec_dot_prod (double * vec_a, double * vec_b, uint64_t n);

/// @brief Calcula el producto cruz entre vectores de 3 dimensiones
/// @param vec_a es el primer vector
/// @param vec_b es el segundo vector
/// @param vec_c es donde se guarda el resultado
void vec3_cross_prod (double * vec_a, double * vec_b, double * vec_c);

/// @brief Construye la matriz identidad
/// @param mat es la matriz
/// @param n es el número de filas y columnas de `mat`
void mat_identity (double * mat, uint64_t n);

/// @brief Transpone una matriz
/// @param mat_a es la matriz
/// @param mat_b es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat`
/// @param n_2 es el número de columnas de `mat_b`
void mat_transpose (double * mat_a, double * mat_b, uint64_t n_1, uint64_t n_2);

/// @brief Suma dos matrices
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a` y `mat_b`
/// @param n_2 es el número de columnas de `mat_a`y `mat_b`
void mat_add (double * mat_a, double * mat_b, double * mat_c, uint64_t n_1, uint64_t n_2);
/// @brief Resta dos matrices
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a` y `mat_b`
/// @param n_2 es el número de columnas de `mat_a`y `mat_b`
void mat_sub (double * mat_a, double * mat_b, double * mat_c, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica un escalar por una matriz
/// @param scalar es el escalar
/// @param mat es la matriz
/// @param n_1 es el número de filas de `mat`
/// @param n_2 es el número de columnas de `mat`
void scalar_mat_mul (double scalar, double * mat, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica una matriz por un vector columna
/// @param mat es la matriz
/// @param vec_a es el vector columna
/// @param vec_b es donde se guarda el resultado
/// @param n_1 es el número de filas de `a`
/// @param n_2 es el número de columnas de `a` y `b`
void mat_vec_mul (double * mat, double * vec_a, double * vec_b, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica dos matrices
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `a`
/// @param n_2 es el número de columnas de `a` y el número de filas de `b`
/// @param n_3 es el número de filas de `b`
void mat_mul (double * mat_a, double * mat_b, double * mat_c, uint64_t n_1, uint64_t n_2, uint64_t n_3);

/// @brief Construye la matriz de vista para una cámara en `position` viendo hacia `target` con vector de referencia hacia arriba `ref`
/// @param position es la posición de la cámara
/// @param target es la posición adonde apunta la cámara
/// @param ref es el vector de referencia hacia arriba
/// @param mat es donde se guarda el resultado
void camera_look_at_mat4 (double * position, double * target, double * ref, double * mat);

/// @brief Construye la matriz de proyección ortogonal
/// @param aspect_ratio es la razón de aspecto del volumen de visión
/// @param ext_v es la extensión vertical del volumen de visión
/// @param near es la distancia al plano cercano
/// @param far es la distancia al plano lejano
/// @param mat es donde se guarda el resultado
void orthogonal_proj_mat4 (double aspect_ratio, double ext_v, double near, double far, double * mat);

/// @brief Construye la matriz de proyección en perspectiva
/// @param aspect_ratio es la razón de aspecto del volumen de visión
/// @param fov_v es el campo de visión vertical
/// @param near es la distancia al plano cercano
/// @param far es la distancia al plano lejano
/// @param mat es donde se guarda el resultado
void perspective_proj_mat4 (double aspect_ratio, double fov_v, double near, double far, double * mat);

/// @brief Construye la matriz de transformación a partir del desplazamiento, rotación y escalamiento.
/// Se considera que la rotación está dada por ángulos de euler ZYX.
/// El orden de la transformación es: escalamiento, rotación alrededor de Z, rotación alrededor de Y, rotación alrededor de X y traslación.
/// @param position es el desplazamiento
/// @param rotation es la rotación en ángulos de euler ZYX
/// @param scale es el escalamiento
/// @param mat es donde se guarda el resultado
void transform_euler_zyx_mat4 (double * position, double * rotation, double * scale, double * mat);

/// @brief Multiplica una secuencia de matrices 4 ⨯ 4
/// @param array es el arreglo con las matrices
/// @param n es el tamaño del arreglo
/// @param mat es donde se guarda el resultado
void mat4_chain_mul (double * * array, uint64_t n, double * mat);