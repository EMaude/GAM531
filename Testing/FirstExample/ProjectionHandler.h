#include "vgl.h"
#include "glm\glm.hpp"

static class ProjectionHandler{
    private:
    GLuint projection_matrix_location_in_vram;

    public:
    void init();
    void draw();
};