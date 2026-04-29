# include "canvas.hpp"

void Canvas::initializeGL () {
    initializeOpenGLFunctions ();
    glEnable (GL_DEPTH_TEST);
    glEnable (GL_CULL_FACE);
    glEnable (GL_BLEND);
    glEnable (GL_NORMALIZE);
    glEnable (GL_LIGHTING);

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLfloat vec [4];
    for (uint64_t i = 0; i < 4; i ++)
        vec [i] = global_ambient [i];
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, vec);

    glGenTextures (NUM_TEXTURES, texture_names);
    for (uint64_t i = 0; i < NUM_TEXTURES; i ++) {
        glBindTexture (GL_TEXTURE_2D, texture_names [i]);
        glTexImage2D (GL_TEXTURE_2D, 0, 4, texture_images [i].width (), texture_images [i].height (), 0, GL_BGRA, GL_UNSIGNED_BYTE, texture_images [i].bits ());
        gluBuild2DMipmaps (GL_TEXTURE_2D, 4, texture_images [i].width (), texture_images [i].height (), GL_BGRA, GL_UNSIGNED_BYTE, texture_images [i].bits ());
    }
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
    glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
}

void Canvas::resizeGL (int w, int h) {
    aspect_ratio = w;
    aspect_ratio /= h;
    draw_commit = true;
}

void Canvas::paintGL () {
    if (! draw_commit)
        return;
    GLfloat vec [4];
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Matriz de vista
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glRotated (- camera_transforms [camera_id].rotation [0] * RAD_TO_DEG, 1, 0, 0);
    glRotated (- camera_transforms [camera_id].rotation [1] * RAD_TO_DEG, 0, 1, 0);
    glRotated (- camera_transforms [camera_id].rotation [2] * RAD_TO_DEG, 0, 0, 1);
    glTranslated (- camera_transforms [camera_id].position [0], - camera_transforms [camera_id].position [1], - camera_transforms [camera_id].position [2]);

    // Luces
    for (uint64_t light = 0; light < NUM_LIGHTS; light ++) {
        if (active_lights & 1lu << light) {
            glEnable (GL_LIGHT0 + light);
            for (uint64_t i = 0; i < 3; i ++)
                vec [i] = lights [light].position [i];
            vec [3] = 1;
            glLightfv (GL_LIGHT0 + light, GL_POSITION, vec);
            for (uint64_t i = 0; i < 4; i ++)
                vec [i] = lights [light].ambient [i];
            glLightfv (GL_LIGHT0 + light, GL_AMBIENT, vec);
            for (uint64_t i = 0; i < 4; i ++)
                vec [i] = lights [light].diffuse [i];
            glLightfv (GL_LIGHT0 + light, GL_DIFFUSE, vec);
            for (uint64_t i = 0; i < 4; i ++)
                vec [i] = lights [light].specular [i];
            glLightfv (GL_LIGHT0 + light, GL_SPECULAR, vec);
        } else
            glDisable (GL_LIGHT0 + light);
    }

    // Matriz de modelo
    glTranslated (object_transforms [object_id].position [0], object_transforms [object_id].position [1], object_transforms [object_id].position [2]);
    glRotated (object_transforms [object_id].rotation [2] * RAD_TO_DEG, 0, 0, 1);
    glRotated (object_transforms [object_id].rotation [1] * RAD_TO_DEG, 0, 1, 0);
    glRotated (object_transforms [object_id].rotation [0] * RAD_TO_DEG, 1, 0, 0);
    glScaled (object_transforms [object_id].scale [0], object_transforms [object_id].scale [1], object_transforms [object_id].scale [2]);

    // Matriz de proyección
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if (perspective_flag)
        gluPerspective (FOV_V * RAD_TO_DEG, aspect_ratio, NEAR, FAR);
    else
        glOrtho (- .5 * EXT_V * aspect_ratio, .5 * EXT_V * aspect_ratio, -.5 * EXT_V, .5 * EXT_V, NEAR, FAR);

    // Material
    for (uint64_t i = 0; i < 4; i ++)
        vec [i] = materials [material_id].ambient [i];
    glMaterialfv (GL_FRONT, GL_AMBIENT, vec);
    for (uint64_t i = 0; i < 4; i ++)
        vec [i] = materials [material_id].diffuse[i];
    glMaterialfv (GL_FRONT, GL_DIFFUSE, vec);
    for (uint64_t i = 0; i < 4; i ++)
        vec [i] = materials [material_id].specular [i];
    glMaterialfv (GL_FRONT, GL_SPECULAR, vec);
    glMaterialf (GL_FRONT, GL_SHININESS, materials [material_id].shininess);

    // Textura
    if (texture_id > 0) {
        glEnable (GL_TEXTURE_2D);
        glBindTexture (GL_TEXTURE_2D, texture_names [texture_id]);
    } else
        glDisable (GL_TEXTURE_2D);

    // Generación de coordenadas de textura
    if (gen_text_coords_flag) {
        glEnable (GL_TEXTURE_GEN_S);
        glEnable (GL_TEXTURE_GEN_T);
    } else {
        glDisable (GL_TEXTURE_GEN_S);
        glDisable (GL_TEXTURE_GEN_T);
    }

    // Dibujar
    glBegin (GL_TRIANGLES);
        for (uint64_t i = 0; i < object_vertices [object_id].size (); i ++) {
            glNormal3dv (object_vertices [object_id] [i].normal);
            glTexCoord2dv (object_vertices [object_id] [i].texture);
            glVertex3dv (object_vertices [object_id] [i].position);
        }
    glEnd ();

    context () -> swapBuffers (context () -> surface ());
    draw_commit = false;
}

void Canvas::center_view () {
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();
    glLoadIdentity ();
    gluLookAt (
        camera_transforms [camera_id].position [0],
        camera_transforms [camera_id].position [1],
        camera_transforms [camera_id].position [2],
        object_transforms [object_id].position [0],
        object_transforms [object_id].position [1],
        object_transforms [object_id].position [2],
        camera_ref [0],
        camera_ref [1],
        camera_ref [2]
    );
    GLdouble mat [4] [4];
    glGetDoublev (GL_MODELVIEW_MATRIX, reinterpret_cast <double *> (mat));
    glPopMatrix ();
    if (mat [2] [0] == 1) {
        camera_transforms [camera_id].rotation [0] = 0;
        camera_transforms [camera_id].rotation [1] = M_PI_2;
        camera_transforms [camera_id].rotation [2] = - atan2 (mat [0] [1], mat [0] [2]);
    } else if (mat [2] [0] == - 1) {
        camera_transforms [camera_id].rotation [0] = 0;
        camera_transforms [camera_id].rotation [1] = - M_PI_2;
        camera_transforms [camera_id].rotation [2] = atan2 (- mat [0] [1], - mat [0] [2]);
    } else {
        camera_transforms [camera_id].rotation [0] = atan2 (mat [2] [1], mat [2] [2]);
        camera_transforms [camera_id].rotation [1] = - asin (mat [2] [0]);
        camera_transforms [camera_id].rotation [2] = atan2 (mat [1] [0], mat [0] [0]);
    }
}
