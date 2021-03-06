/* Copyright 2017 Floris Creyf
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stem.h"
#include <limits>
#include <cmath>

using namespace pg;

Stem::Stem(Stem *parent) :
	nextSibling(nullptr),
	prevSibling(nullptr),
	child(nullptr),
	parent(parent),
	depth(0),
	sectionDivisions(4),
	radiusCurve(0),
	material{0, 0},
	distance(0.0f),
	minRadius(0.0f),
	maxRadius(0.0f),
	swelling(1.5f, 3.0f),
	location(0.0f, 0.0f, 0.0f),
	custom(false)
{
	if (parent)
		this->depth = parent->depth + 1;
}

Stem::~Stem()
{

}

Stem::Stem(const Stem &original) :
	nextSibling(original.nextSibling),
	prevSibling(original.prevSibling),
	child(original.child),
	parent(original.parent),
	leaves(original.leaves),
	joints(original.joints),
	depth(original.depth),
	sectionDivisions(original.sectionDivisions),
	radiusCurve(original.radiusCurve),
	material{original.material[0], original.material[1]},
	distance(original.distance),
	minRadius(original.minRadius),
	maxRadius(original.maxRadius),
	swelling(original.swelling),
	location(original.location),
	path(original.path),
	custom(original.custom),
	parameterTree(original.parameterTree)
{

}

Stem &Stem::operator=(const Stem &stem)
{
	this->parent = stem.parent;
	this->child = stem.child;
	this->nextSibling = stem.nextSibling;
	this->prevSibling = stem.prevSibling;
	this->depth = stem.depth;
	this->maxRadius = stem.maxRadius;
	this->minRadius = stem.minRadius;
	this->radiusCurve = stem.radiusCurve;
	this->path = stem.path;
	this->sectionDivisions = stem.sectionDivisions;
	this->distance = stem.distance;
	this->location = stem.location;
	this->material[0] = stem.material[0];
	this->material[1] = stem.material[1];
	this->leaves = stem.leaves;
	this->swelling = stem.swelling;
	this->custom = stem.custom;
	this->parameterTree = stem.parameterTree;
	return *this;
}

bool Stem::operator==(const Stem &stem) const
{
	return (
		this->depth == stem.depth &&
		this->radiusCurve == stem.radiusCurve &&
		this->minRadius == stem.minRadius &&
		this->maxRadius == stem.maxRadius &&
		this->path == stem.path &&
		this->sectionDivisions == stem.sectionDivisions &&
		this->distance == stem.distance &&
		this->location == stem.location &&
		this->material[0] == stem.material[0] &&
		this->material[1] == stem.material[1] &&
		this->swelling == stem.swelling &&
		this->leaves == stem.leaves &&
		this->custom == stem.custom);
}

void Stem::init(Stem *parent)
{
	this->joints.clear();
	this->leaves.clear();
	this->maxRadius = 0.0f;
	this->minRadius = 0.0f;
	this->radiusCurve = 0;
	this->swelling = Vec2(1.2f, 3.0f);
	this->material[0] = 0;
	this->material[1] = 0;
	this->sectionDivisions = 8;
	this->custom = false;
	this->parameterTree.reset();
	this->nextSibling = nullptr;
	this->prevSibling = nullptr;
	this->child = nullptr;
	this->parent = parent;
	this->distance = 0.0f;
	this->location = Vec3(0.0f, 0.0f, 0.0f);
	this->path.setInitialDivisions(0);
	this->path.setDivisions(0);
	if (parent == nullptr)
		this->depth = 0;
	else
		this->depth = parent->depth + 1;
}

bool Stem::operator!=(const Stem &stem) const
{
	return !(*this == stem);
}

void Stem::setCustom(bool custom)
{
	this->custom = custom;
}

bool Stem::isCustom() const
{
	return this->custom;
}

void Stem::setParameterTree(ParameterTree parameterTree)
{
	this->parameterTree = parameterTree;
}

ParameterTree Stem::getParameterTree() const
{
	return this->parameterTree;
}

GeneratorState *Stem::getState()
{
	return &this->state;
}

size_t Stem::addLeaf(const Leaf &leaf)
{
	this->leaves.push_back(leaf);
	return this->leaves.size() - 1;
}

void Stem::insertLeaf(const Leaf &leaf, size_t index)
{
	this->leaves.insert(this->leaves.begin() + index, leaf);
}

size_t Stem::getLeafCount() const
{
	return this->leaves.size();
}

Leaf *Stem::getLeaf(size_t index)
{
	return &this->leaves.at(index);
}

const Leaf *Stem::getLeaf(size_t index) const
{
	return &this->leaves.at(index);
}

const std::vector<Leaf> &Stem::getLeaves() const
{
	return this->leaves;
}

void Stem::removeLeaf(size_t index)
{
	this->leaves.erase(this->leaves.begin() + index);
}

void Stem::setSectionDivisions(int divisions)
{
	this->sectionDivisions = divisions;
}

int Stem::getSectionDivisions() const
{
	return this->sectionDivisions;
}

void Stem::setCollarDivisions(int divisions)
{
	this->path.setInitialDivisions(divisions);
	this->path.generate();
}

int Stem::getCollarDivisions() const
{
	return this->path.getInitialDivisions();
}

void Stem::setPath(Path &path)
{
	this->path = path;
	this->path.generate();
	updatePositions(this);
}

const Path &Stem::getPath() const
{
	return this->path;
}

void Stem::updatePositions(Stem *stem)
{
	Stem *child = stem->child;
	while (child != nullptr) {
		child->setDistance(child->distance);
		updatePositions(child);
		child = child->nextSibling;
	}
}

void Stem::setDistance(float position)
{
	if (this->parent != nullptr) {
		Path parentPath = this->parent->getPath();
		Vec3 point = parentPath.getIntermediate(position);
		if (std::isnan(point.x))
			this->location = point;
		else
			this->location = parent->getLocation() + point;
		this->distance = position;
		updatePositions(this);
	}
}

float Stem::getDistance() const
{
	return this->distance;
}

Vec3 Stem::getLocation() const
{
	return this->location;
}

void Stem::setMaterial(Stem::Type feature, unsigned material)
{
	this->material[feature] = material;
}

unsigned Stem::getMaterial(Stem::Type feature) const
{
	return this->material[feature];
}

Stem *Stem::getParent()
{
	return this->parent;
}

const Stem *Stem::getParent() const
{
	return this->parent;
}

Stem *Stem::getSibling()
{
	return this->nextSibling;
}

const Stem *Stem::getSibling() const
{
	return this->nextSibling;
}

Stem *Stem::getChild()
{
	return this->child;
}

const Stem *Stem::getChild() const
{
	return this->child;
}

void Stem::getFork(Stem *fork[2]) const
{
	float length = this->path.getLength();
	Stem *child = this->child;
	fork[0] = nullptr;
	fork[1] = nullptr;
	while (child) {
		bool isFork = child->getDistance() >= length;
		if (isFork && !fork[0])
			fork[0] = child;
		else if (isFork) {
			fork[1] = child;
			break;
		}
		child = child->getSibling();
	}
	if (!fork[1])
		fork[0] = nullptr;
}

bool Stem::isDescendantOf(Stem *stem) const
{
	const Stem *descendant = this;
	if (this->depth > stem->getDepth()) {
		while (descendant != nullptr) {
			if (stem == descendant->parent)
				return true;
			descendant = descendant->parent;
		}
	}
	return false;
}

int Stem::getDepth() const
{
	return this->depth;
}

void Stem::setMaxRadius(float radius)
{
	this->maxRadius = radius;
}

float Stem::getMaxRadius() const
{
	return this->maxRadius;
}

void Stem::setMinRadius(float radius)
{
	this->minRadius = radius;
}

float Stem::getMinRadius() const
{
	return this->minRadius;
}

void Stem::setRadiusCurve(unsigned index)
{
	this->radiusCurve = index;
}

unsigned Stem::getRadiusCurve() const
{
	return this->radiusCurve;
}

void Stem::setSwelling(Vec2 scale)
{
	this->swelling = scale;
}

Vec2 Stem::getSwelling() const
{
	return this->swelling;
}

std::vector<Joint> Stem::getJoints() const
{
	return this->joints;
}

bool Stem::hasJoints() const
{
	return !this->joints.empty();
}

void Stem::addJoint(Joint joint)
{
	joint.updateLocation(path.get(joint.getPathIndex()));
	this->joints.push_back(joint);
}

void Stem::clearJoints()
{
	this->joints.clear();
	Stem *child = this->child;
	while (child) {
		child->clearJoints();
		child = child->nextSibling;
	}
}
