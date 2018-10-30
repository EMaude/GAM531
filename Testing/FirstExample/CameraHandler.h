#include "vgl.h"
#include "glm\glm.hpp"

static class CameraHandler{
    private:
    glm::vec3 m_pos;
    GLuint cam_matrix_location_in_vram;

    public:

    void init();
    void draw();
    void update(glm::vec3 pos);
    glm::vec3 getPos();
};