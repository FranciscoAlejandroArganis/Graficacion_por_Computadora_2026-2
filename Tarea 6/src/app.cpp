# include "app.hpp"

# define TINYOBJLOADER_IMPLEMENTATION
# include "tiny_obj_loader.h"

Transform object_transforms [] = {
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 3.14, 0 },
        .scale = { 15, 15, 15 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 4.71, 0 },
        .scale = { 75, 75, 75 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 3.14, 0 },
        .scale = { 175, 175, 175 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 0, 0 },
        .scale = { 10, 10, 10 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 1.57, 0, 1.57 },
        .scale = { 1, 1, 1 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 0, 0 },
        .scale = { 10, 10, 10 }
    },
    {
        .position = { 0, 0, 0 },
        .rotation = { 0, 3.14, 0 },
        .scale = { .24, .24, .24 }
    }
};
Material materials [] = {
    {
        .ambient = { 0, 0, 0, 1 },
        .diffuse = { .5, .5, .5, 1 },
        .specular = { .7, .7, .7, 1 },
        .shininess = 32
    },
    {
        .ambient = { .23125, .23125, .23125, 1 },
        .diffuse = { .2775, .2775, .2775, 1 },
        .specular = { .773911, .773911, .773911, 1 },
        .shininess = 89.6
    }
};
CameraTransform camera_transforms [] = {
    {
        .position = { 11.3, 61.1, - 62 },
        .rotation = { 2.37, .18, 3.14 }
    },
    {
        .position = { - 55.7, 57.3, 19 },
        .rotation { - .77, - 1.24, 0 }
    },
    {
        .position = { 50, 65.5, 34.6 },
        .rotation = { - .82, .97, 0 }
    }
};
double camera_ref [] = { 0, 1, 0 };
bool perspective_flag = true;
bool gen_text_coords_flag = false;
Light lights [] = {
    {
        .position = { 15.3, 70, - 37.4 },
        .ambient = { .1, .1, .1, 1 },
        .diffuse = { 1, 1, 1, 1 },
        .specular = { 1, 1, 1, 1 }
    },
    {
        .position = { - 2, 70, 32.6 },
        .ambient = { .04, .16, .2, 1 },
        .diffuse = { .2, .8, 1, 1 },
        .specular = { .2, .8, 1, 1 }
    }
};
double global_ambient [] = { .1, .1, .1, 1 };
uint64_t active_lights = 3;
uint64_t camera_id = 0;
uint64_t skybox_id = 0;
uint64_t object_id = 0;
uint64_t light_id = 0;
uint64_t material_id = 0;
uint64_t texture_id = 0;
double aspect_ratio;
bool draw_commit;
std::vector <Vertex> object_vertices [NUM_OBJECTS];
QImage texture_images [NUM_TEXTURES];
GLuint texture_names [NUM_TEXTURES];

bool load_obj (std::string file) {
    tinyobj::ObjReader reader;
    if (! reader.ParseFromFile (file))
        return false;
    tinyobj::attrib_t attrib = reader.GetAttrib ();
    tinyobj::real_t (* positions) [3] = reinterpret_cast <tinyobj::real_t (*) [3]> (attrib.vertices.data ());
    tinyobj::real_t (* normals) [3] = reinterpret_cast <tinyobj::real_t (*) [3] > (attrib.normals.data ());
    tinyobj::real_t (* texture_coords) [2] = reinterpret_cast <tinyobj::real_t (*) [2] > (attrib.texcoords.data ());
    for (tinyobj::shape_t shape : reader.GetShapes ()) {
        tinyobj::index_t (* indices) [3] = reinterpret_cast <tinyobj::index_t (*) [3]> (shape.mesh.indices.data ());
        for (uint64_t i = 0; i < shape.mesh.num_face_vertices.size (); i ++) {
            bool missing_normal_data = false;
            bool missing_texture_data = false;
            for (uint64_t j = 0; j < 3; j ++) {
                int64_t index = indices [i] [j].vertex_index;
                Vertex vertex = { .position = { positions [index] [0], positions [index] [1], positions [index] [2] } };
                index = indices [i] [j].normal_index;
                if (index >= 0) {
                    vertex.normal [0] = normals [index] [0];
                    vertex.normal [1] = normals [index] [1];
                    vertex.normal [2] = normals [index] [2];
                } else
                    missing_normal_data = true;
                index = indices [i] [j].texcoord_index;
                if (index >= 0) {
                    vertex.texture [0] = texture_coords [index] [0];
                    vertex.texture [1] = texture_coords [index] [1];
                } else
                    missing_texture_data = true;
                object_vertices [object_id].push_back (vertex);
            }
            Vertex * triangle = object_vertices [object_id].data () + 3 * i;
            if (missing_normal_data) {
                triangle [0].normal [0] = triangle [0].position [1] * (triangle [1].position [2] - triangle [2].position [2]) - triangle [0].position [2] * (triangle [1].position [1] - triangle [2].position [1]) + triangle [1].position [1] * triangle [2].position [2] - triangle [2].position [1] * triangle [1].position [2];
                triangle [0].normal [1] = - triangle [0].position [0] * (triangle [1].position [2] - triangle [2].position [2]) + triangle [0].position [2] * (triangle [1].position [0] - triangle [2].position [0]) - triangle [1].position [0] * triangle [2].position [2] + triangle [2].position [0] * triangle [1].position [2];
                triangle [0].normal [2] = triangle [0].position [0] * (triangle [1].position [1] - triangle [2].position [1]) - triangle [0].position [1] * (triangle [1].position [0] - triangle [2].position [0]) + triangle [1].position [0] * triangle [2].position [1] - triangle [2].position [0] * triangle [1].position [1];
                memcpy (triangle [1].normal, triangle [0].normal, 3 * sizeof (double));
                memcpy (triangle [2].normal, triangle [0].normal, 3 * sizeof (double));
            }
            if (missing_texture_data) {
                if (i & 1) {
                    triangle [0].texture [0] = 0;
                    triangle [0].texture [1] = 1;
                    triangle [1].texture [0] = 1;
                    triangle [1].texture [1] = 1;
                    triangle [2].texture [0] = 1;
                    triangle [2].texture [1] = 0;
                } else {
                    triangle [0].texture [0] = 0;
                    triangle [0].texture [1] = 0;
                    triangle [1].texture [0] = 0;
                    triangle [1].texture [1] = 1;
                    triangle [2].texture [0] = 1;
                    triangle [2].texture [1] = 0;
                }
            }
        }
    }
    return true;
}

bool load_texture (std::string file) {
    texture_images [texture_id] = QImage (file.c_str ());
    if (texture_images [texture_id].isNull ())
        return false;
    return true;
}