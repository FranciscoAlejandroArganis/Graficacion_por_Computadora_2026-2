/// @file gmath.hpp

# pragma once

# include <cstdint>
# include <cstring>
# include <cmath>

/// @brief Limita un valor `x` a un intervalo `[a,b]`
/// @param x es el valor
/// @param a es el límite inferior del intervalo
/// @param b es el límite superior del intervalo
/// @return `x` si se encuentra en `[a,b]` o el límite más cercano a `x` si no está en el intervalo
double clamp (double x, double a, double b);

/// @brief Realiza una interpolación lineal para `t`
/// @param t es el valor de interpolación
/// @param a es el resultado si `t = 0`
/// @param b es el resultado si `t = 1`
/// @return la interpolación lineal para `t`
double lerp (double t, double a, double b);

/// @brief Realiza una interpolación lineal para `t`
/// @param t es el valor de interpolación
/// @param a es el resultado si `t = -1`
/// @param b es el resultado si `t = 1`
/// @return la interpolación lineal para `t`
double dlerp (double t, double a, double b);

/// @brief Realiza una interpolación lineal para `u` y `v`
/// @param u es el primer valor de interpolación
/// @param v es el segundo valor de interpolación
/// @param a es el resultado si `u = 0` y `v = 0`
/// @param b es el resultado si `u = 1` y `v = 0`
/// @param c es el resultado si `u = 0` y `v = 1`
/// @return la interpolación lineal para `u` y `v`
double blerp (double u, double v, double a, double b, double c);

/// @brief Regresa la parte fraccionaria no negativa de `x`
/// @param x es el valor
/// @return la parte fraccionaria no negativa de `x`
double frac_part (double x);

/// @brief Realiza una interpolación multilineal en `n` dimensiones.
/// @param n es el número de dimensioines
/// @param target es el valor de interpolación
/// @param inputs es un arreglo con los pares de valores de cada variable de entrada
/// @param outputs es un arreglo con los valores de la variable de salida correspondientes a cada combinación de los pares de entradas
/// @return la interpolación multilineal para `target`
double multilinear_interpolation (uint64_t n, double * target, double (* inputs) [2], double * outputs);

/// @brief Calcula la norma de un vector
/// @param vec es el vector
/// @param n es la dimensión de `vec`
/// @return la norma de `vec`
double vec_norm (double * vec, uint64_t n);

/// @brief Construye el vector unitario en la dirección del vector `vec`
/// @param vec_a es el vector
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_unit (double * vec_a, double * vec_b, uint64_t n);

/// @brief Convierte un vector en coordenadas normales a coordenadas homogéneas
/// @param vec_a es el vector en coordenadas normales
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_to_homogeneous (double * vec_a, double * vec_b, uint64_t n);

/// @brief Convierte un vector en coordenadas homogeneas a coordenadas normales
/// @param vec_a es el vector en coordenadas homogéneas
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_from_homogeneous (double * vec_a, double * vec_b, uint64_t n);

/// @brief Construye la negación de un vector
/// @param vec_a es el vector
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_neg (double * vec_a, double * vec_b, uint64_t n);

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

/// @brief Multiplica dos vectores elemento por elemento
/// @param vec_a es el primer vector
/// @param vec_b es el segundo vector
/// @param vec_c es donde se guarda el resultado
/// @param n es la dimensión de `vec_a` y `vec_b`
void vec_emul (double * vec_a, double * vec_b, double * vec_c, uint64_t n);

/// @brief Multiplica un vector por un escalar
/// @param vec_a es el vector
/// @param scalar es el escalar
/// @param vec_b es donde se guarda el resultado
/// @param n es la dimensión de `vec_a`
void vec_scalar_mul (double * vec_a, double scalar, double * vec_b, uint64_t n);

/// @brief Calcula el producto punto entre dos vectores
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

/// @brief Construye el vector unitario reflejado
/// @param incident es el vector unitario incidente
/// @param normal es el vector unitario de la normal
/// @param reflected es donde se guarda el resultado
/// @param n es la dimensión de `incident` y `normal`
void vec_reflect (double * incident, double * normal, double * reflected, uint64_t n);

/// @brief Construye la matriz identidad
/// @param mat es donde se guarda el resultado
/// @param n es el número de filas y columnas de la matriz
void mat_identity (void * mat, uint64_t n);

/// @brief Construye la transpuesta de una matriz
/// @param mat_a es la matriz
/// @param mat_b es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a`
/// @param n_2 es el número de columnas de `mat_a`
void mat_transpose (void * mat_a, void * mat_b, uint64_t n_1, uint64_t n_2);

/// @brief Suma dos matrices
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a` y `mat_b`
/// @param n_2 es el número de columnas de `mat_a`y `mat_b`
void mat_add (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2);

/// @brief Resta dos matrices
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a` y `mat_b`
/// @param n_2 es el número de columnas de `mat_a`y `mat_b`
void mat_sub (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica dos matrices elemento por elemento
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a` y `mat_b`
/// @param n_2 es el número de columnas de `mat_a`y `mat_b`
void mat_emul (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica una matriz por un escalar
/// @param mat_a es la matriz
/// @param scalar es el escalar
/// @param mat_b es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a`
/// @param n_2 es el número de columnas de `mat_a`
void mat_scalar_mul (void * mat_a, double scalar, void * mat_b, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica una matriz por un vector columna
/// @param mat es la matriz
/// @param vec_a es el vector columna
/// @param vec_b es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat`
/// @param n_2 es el número de columnas de `mat` y `vec_a`
void mat_vec_mul (void * mat, double * vec_a, double * vec_b, uint64_t n_1, uint64_t n_2);

/// @brief Multiplica dos matrices
/// @param mat_a es la primer matriz
/// @param mat_b es la segunda matriz
/// @param mat_c es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a`
/// @param n_2 es el número de columnas de `mat_a` y el número de filas de `mat_b`
/// @param n_3 es el número de columnas de `mat_b`
void mat_mul (void * mat_a, void * mat_b, void * mat_c, uint64_t n_1, uint64_t n_2, uint64_t n_3);

/// @brief Construye la inversa de una matriz. Se pierde el contenido de `mat_a`.
/// @param mat_a es la matriz
/// @param mat_b es donde se guarda el resultado
/// @param n es el número de filas y columnas de `mat_a`
/// @return el determinante de `mat_a`
double mat_inverse (void * mat_a, void * mat_b, uint64_t n);

/// @brief Construye la matriz de vista para una cámara en `position` viendo hacia `target` con vector de referencia hacia arriba `ref`
/// @param position es la posición de la cámara
/// @param target es la posición adonde apunta la cámara
/// @param ref es el vector de referencia hacia arriba
/// @param mat es donde se guarda el resultado
void camera_look_at_mat4 (double * position, double * target, double * ref, void * mat);

/// @brief Construye la matriz de proyección ortogonal
/// @param aspect_ratio es la razón de aspecto del volumen de visión
/// @param ext_v es la extensión vertical del volumen de visión
/// @param near es la distancia al plano cercano
/// @param far es la distancia al plano lejano
/// @param mat es donde se guarda el resultado
void orthogonal_proj_mat4 (double aspect_ratio, double ext_v, double near, double far, void * mat);

/// @brief Construye la inversa de la matriz de proyección ortogonal
/// @param aspect_ratio es la razón de aspecto del volumen de visión
/// @param ext_v es la extensión vertical del volumen de visión
/// @param near es la distancia al plano cercano
/// @param far es la distancia al plano lejano
/// @param mat es donde se guarda el resultado
void inv_orthogonal_proj_mat4 (double aspect_ratio, double ext_v, double near, double far, void * mat);

/// @brief Construye la matriz de proyección en perspectiva
/// @param aspect_ratio es la razón de aspecto del volumen de visión
/// @param fov_v es el campo de visión vertical
/// @param near es la distancia al plano cercano
/// @param far es la distancia al plano lejano
/// @param mat es donde se guarda el resultado
void perspective_proj_mat4 (double aspect_ratio, double fov_v, double near, double far, void * mat);

/// @brief Construye la inversa de la matriz de proyección en perspectiva
/// @param aspect_ratio es la razón de aspecto del volumen de visión
/// @param fov_v es el campo de visión vertical
/// @param near es la distancia al plano cercano
/// @param far es la distancia al plano lejano
/// @param mat es donde se guarda el resultado
void inv_perspective_proj_mat4 (double aspect_ratio, double fov_v, double near, double far, void * mat);

/// @brief Construye la matriz de transformación a partir del desplazamiento, rotación y escalamiento.
/// Se considera que la rotación está dada por ángulos de euler ZYX.
/// El orden de la transformación es: escalamiento, rotación alrededor de Z, rotación alrededor de Y, rotación alrededor de X y traslación.
/// @param position es el desplazamiento
/// @param rotation es la rotación en ángulos de euler ZYX
/// @param scale es el escalamiento
/// @param mat es donde se guarda el resultado
void transform_euler_zyx_mat4 (double * position, double * rotation, double * scale, void * mat);

/// @brief Construye la matriz de vista a partir de la transformación de la cámara
/// @param position es la posición de la cámara
/// @param rotation es la rotación de la cámara en ángulos de euler ZYX
/// @param mat es donde se guarda el resultado
void camera_euler_zyx_mat4 (double * position, double * rotation, void * mat);

/// @brief Extrae los ángulos de euler ZYX de una matriz de rotación
/// @param mat es la matriz de rotación
/// @param rotation es donde se guardan los ángulos de euler ZYX
void mat4_to_euler_zyx (void * mat, double * rotation);

/// @brief Multiplica una secuencia de matrices 4 ⨯ 4
/// @param array es el arreglo con las matrices
/// @param n es el tamaño del arreglo
/// @param mat es donde se guarda el resultado
void mat4_chain_mul (void * * array, uint64_t n, void * mat);

/// @brief Calcula la caja delimitadora alineada con los ejes mínima para los puntos en las filas de la matriz `mat`
/// @param mat es la matriz con los puntos
/// @param aabb es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat`
/// @param n_2 es el número de columnas de `mat`
void bound_points (void * mat, void * aabb, uint64_t n_1, uint64_t n_2);

/// @brief Construye una parametrización lineal para los puntos en las filas de la matriz `mat_a`
/// @param mat_a es la matriz con los puntos
/// @param mat_b es donde se guarda el resultado
/// @param n_1 es el número de filas de `mat_a`
/// @param n_2 es el número de columnas de `mat_a`
void linear_param (void * mat_a, void * mat_b, uint64_t n_1, uint64_t n_2);

/// @brief Determina si una caja delimitadora alineada con los ejes es vacía
/// @param aabb es el caja
/// @param n es la dimensión de `aabb`
/// @return `true` si y solo si la caja está vacía
bool aabb_empty (void * aabb, uint64_t n);

/// @brief Intersecta dos cajas delimitadoras alineadas con los ejes
/// @param aabb_a es la primer caja
/// @param aabb_b es la segunda caja
/// @param aabb_c es donde se guarda el resultado
/// @param n es la dimensión de `aabb_a` y `aabb_b`
void aabb_intersection (void * aabb_a, void * aabb_b, void * aabb_c, uint64_t n);

/// @brief Calcula la normal de la recta que pasa por dos puntos en el plano
/// @param point_a es el primer punto
/// @param point_b es el segundo punto
/// @param normal es donde se guarda el resultado
void line_normal (double * point_a, double * point_b, double * normal);

/// @brief Calcula la normal del plano que pasa por tres puntos en el espacio
/// @param point_a es el primer punto
/// @param point_b es el segundo punto
/// @param point_c es el tercer punto
/// @param normal es donde se guarda el resultado
void plane_normal (double * point_a, double * point_b, double * point_c, double * normal);
