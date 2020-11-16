/* Plant Generator
 * Copyright (C) 2019  Floris Creyf
 *
 * Plant Generator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Plant Generator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "material_viewer.h"
#include "plant_generator/math/vec3.h"
#include "plant_generator/math/mat4.h"

using pg::Vec3;
using pg::Mat4;

MaterialViewer::MaterialViewer(SharedResources *shared, QWidget *parent) :
	QOpenGLWidget(parent)
{
	this->shared = shared;
	this->materialIndex = 0;
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	camera.setTarget(Vec3(0.5f, 0.0f, 0.5f));
	camera.setOrientation(180.0f, -180.0f);
	camera.setDistance(0.8f);
	camera.setPanSpeed(0.004f);
	camera.setZoom(0.01f, 0.3f, 2.0f);
}

QSize MaterialViewer::sizeHint() const
{
	return QSize(width(), 200);
}

void MaterialViewer::updateMaterial(unsigned materialIndex)
{
	this->materialIndex = materialIndex;
	update();
}

void MaterialViewer::createInterface()
{
	Geometry plane;
	Vec3 a(1.0f, 0.0f, 0.0f);
	Vec3 b(0.0f, 0.0f, 1.0f);
	Vec3 c(0.0f, 0.0f, 0.0f);
	Vec3 normal(0.0f, 0.0f, 1.0f);
	Vec3 tangent(0.0f, 1.0f, 0.0f);
	plane.addPlane(a, b, c, normal, tangent);
	planeSegment = plane.getSegment();
	buffer.update(plane);
}

void MaterialViewer::initializeGL()
{
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
	glDepthRange(0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PRIMITIVE_RESTART);
	buffer.initialize(GL_DYNAMIC_DRAW);
	buffer.allocatePointMemory(20);
	buffer.allocateIndexMemory(10);
	glPrimitiveRestartIndex(Geometry::primitiveReset);
	createInterface();
	shared->initialize();
}

void MaterialViewer::paintGL()
{
	glClearColor(0.22f, 0.22f, 0.22f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->camera.updateVP();
	ShaderParams params = shared->getMaterial(this->materialIndex);
	pg::Material material = params.getMaterial();

	float aspect = params.getMaterial().getRatio();
	Mat4 scale;
	if (aspect < 1.0f)
		scale = pg::scale(Vec3(1.0f, 1.0f/aspect, 1.0f));
	else
		scale = pg::scale(Vec3(aspect, 1.0f, 1.0f));
	buffer.use();
	glUseProgram(shared->getShader(SharedResources::Material));

	Mat4 projection = scale * this->camera.getVP();
	Vec3 position = this->camera.getPosition();
	glUniformMatrix4fv(0, 1, GL_FALSE, &projection[0][0]);
	glUniform3f(1, position.x, position.y, position.z);

	float shininess = material.getShininess();
	Vec3 ambient = material.getAmbient();
	glUniform3f(2, ambient.x, ambient.y, ambient.z);
	glUniform1f(3, shininess);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, params.getTexture(pg::Material::Albedo));
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, params.getTexture(pg::Material::Opacity));
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, params.getTexture(pg::Material::Specular));
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, params.getTexture(pg::Material::Normal));

	auto size = sizeof(unsigned);
	GLvoid *start = (GLvoid *)(planeSegment.istart * size);
	GLsizei count = planeSegment.icount;
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, start);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glFlush();
}

void MaterialViewer::resizeGL(int width, int height)
{
	float ratio = static_cast<float>(width) / static_cast<float>(height);
	camera.setWindowSize(width, height);
	camera.setOrthographic(
		Vec3(-ratio, -1.0f, 0.0f), Vec3(ratio, 1.0f, 100.0f));
}

void MaterialViewer::mousePressEvent(QMouseEvent *event)
{
	QPoint p = event->pos();
	if (event->button() == Qt::MidButton) {
		camera.setStartCoordinates(p.x(), p.y());
		if (event->modifiers() & Qt::ControlModifier)
			camera.setAction(Camera::Zoom);
		else if (event->modifiers() & Qt::ShiftModifier)
			camera.setAction(Camera::Pan);
	}
}

void MaterialViewer::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MidButton)
		camera.setAction(Camera::None);
}

void MaterialViewer::mouseMoveEvent(QMouseEvent *event)
{
	QPoint p = event->pos();
	camera.executeAction(p.x(), p.y());
	update();
}

void MaterialViewer::wheelEvent(QWheelEvent *event)
{
	QPoint a = event->angleDelta();
	if (!a.isNull()) {
		float y = a.y() / 10.0f;
		if (y != 0.0f) {
			camera.zoom(y);
			update();
		}
	}
	event->accept();
}
