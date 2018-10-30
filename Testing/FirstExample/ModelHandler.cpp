#include "ModelHandler.h"

void ModelHandler::init(GLuint program)
{
	rotate_value = 0.0f;
	location = glGetUniformLocation(program, "model_matrix");
}

void ModelHandler::draw()
{
	//Creating rotation matrix using glm livrary. In this case, rotating rotat_value degree about Z axis (0, 0, 1)

	glm::mat4 model_view = glm::scale(glm::mat4(1.0), glm::vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Starting the pipeline
	//Rendering the sun
	//glDrawArrays(GL_TRIANGLE_FAN, 0, generated_vertices);
	draw();

	//Transformations required to render the earth
	model_view = glm::rotate(model_view, rotate_value, glm::vec3(0.0, 0.0, 1.0f));
	model_view = glm::translate(model_view, glm::vec3(20.0, 0.0, 0.0));
	model_view = glm::rotate(model_view, 4 * rotate_value, glm::vec3(0.0, 0.0, 1.0f));
	model_view = glm::scale(model_view, glm::vec3(0.5, 0.5, 0.5));

	//Sending the generated transformation matrix to the vertex shader
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Rendering the earth
	//glDrawArrays(GL_TRIANGLE_FAN, 0, generated_vertices);
	draw();


	//Transformations required to render the moon
	model_view = glm::scale(model_view, glm::vec3(0.7, 0.7, 0.7));
	model_view = glm::rotate(model_view, -20 * rotate_value, glm::vec3(0.0, 0.0, 1.0f));
	model_view = glm::translate(model_view, glm::vec3(10.0, 0.0, 0.0));
	model_view = glm::rotate(model_view, -30 * rotate_value, glm::vec3(0.0, 0.0, 1.0f));

	//Sending the generated transformation matrix to the vertex shader
	glUniformMatrix4fv(location, 1, GL_FALSE, &model_view[0][0]);

	//Rendering the moon
	//glDrawArrays(GL_TRIANGLE_FAN, 0, generated_vertices);
	draw();



}
