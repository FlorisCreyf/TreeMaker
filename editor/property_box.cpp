/*
 * Copyright (C) 2016 Floris Creyf
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include "property_box.h"
#include <stdio.h>

PropertyBox::PropertyBox(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	QVBoxLayout *groupLayout;
	QGroupBox *groupBox;
	curveEditor = NULL;
	activeCurve = NULL;
	layout->setSizeConstraint(QLayout::SetMinimumSize);

	groupBox = new QGroupBox(tr("Tree"));
	groupLayout = new QVBoxLayout(groupBox);
	global = new QTableWidget(this);
	crownBaseHeight = new QDoubleSpinBox;
	apicalDominance = new QDoubleSpinBox;
	global->setRowCount(2);
	global->setColumnCount(2);
	apicalDominance->setEnabled(false);
	crownBaseHeight->setEnabled(false);
	global->setCellWidget(0, 0, new QLabel(tr("Crown Base Height")));
	global->setCellWidget(0, 1, crownBaseHeight);
	global->setCellWidget(1, 0, new QLabel(tr("Apical Dominance")));
	global->setCellWidget(1, 1, apicalDominance);
	configureTable(global);
	groupLayout->addStretch(1);
	groupLayout->addWidget(global);
	layout->addWidget(groupBox);

	localGroup = new QGroupBox(tr("Branch"));
	groupLayout = new QVBoxLayout(localGroup);
	local = new QTableWidget(this);
	resolution = new QSpinBox;
	radius = new QDoubleSpinBox;
	radiusCB = new CurveButtonWidget("Radius", this);
	sections = new QSpinBox;
	branches = new QDoubleSpinBox;
	local->setRowCount(4);
	local->setColumnCount(3);
	radius->setSingleStep(0.01);
	sections->setMinimum(2);
	resolution->setMinimum(5);
	local->setCellWidget(0, 0, new QLabel(tr("Radius")));
	local->setCellWidget(0, 1, radius);
	local->setCellWidget(0, 2, createCenteredWidget(radiusCB));
	local->setCellWidget(1, 0, new QLabel(tr("Resolution")));
	local->setCellWidget(1, 1, resolution);
	local->setCellWidget(2, 0, new QLabel(tr("Cross Sections")));
	local->setCellWidget(2, 1, sections);
	local->setCellWidget(3, 0, new QLabel(tr("Branches / Unit")));
	local->setCellWidget(3, 1, branches);
	configureTable(local);
	groupLayout->addStretch(1);
	groupLayout->addWidget(local);
	layout->addWidget(localGroup);
	localGroup->hide();
	
	layout->addStretch(1);
}

void PropertyBox::configureTable(QTableWidget *table)
{
	QHeaderView *header;

	table->horizontalHeader()->hide();
	table->verticalHeader()->hide();
	table->setShowGrid(false);
	table->setFocusPolicy(Qt::NoFocus);
	table->setEditTriggers(QAbstractItemView::NoEditTriggers);
	table->setSelectionMode(QAbstractItemView::NoSelection);

	table->setMaximumHeight(24*table->rowCount());
	for (int i = 0; i < table->rowCount(); i++)
		table->setRowHeight(i, 24);

	header = table->horizontalHeader();
	if (table->columnCount() == 3)
		header->setSectionResizeMode(2, QHeaderView::Fixed);
	header->setSectionResizeMode(1, QHeaderView::Stretch);
	header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
	header->resizeSection(2, 30);
}

QWidget *PropertyBox::createCenteredWidget(QWidget *widget)
{
	QWidget *w = new QWidget(this);
	QHBoxLayout *b = new QHBoxLayout(w);
	b->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
	b->setMargin(0);
	b->addWidget(widget);
	return w;
}

QSize PropertyBox::sizeHint() const
{
	return QSize(300, 0);
}

void PropertyBox::setCurve(vector<tm_vec3> controls, QString name)
{
	if (name == "Radius") {
		radiusCB->getCurveButton()->setControls(controls);
		sceneEditor->changeRadiusCurve(controls);
	}
}

void PropertyBox::toggleCurve(CurveButtonWidget *w)
{
	activeCurve = w;
	curveEditor->setCurve(w->getCurveButton()->getControls(), w->getName());
}

void PropertyBox::fill(tm_tree tree, int branch)
{
	crownBaseHeight->setValue(tm_get_crown_base_height(tree));

	if (branch < 0) {
		localGroup->hide();
		curveEditor->setEnabled(false);
		return;
	}

	resolution->setValue(tm_get_resolution(tree, branch));
	sections->setValue(tm_get_cross_sections(tree, branch));
	radius->setValue(tm_get_radius(tree, branch));
	branches->setValue(tm_get_branch_density(tree, branch));
	fillCurveButtons(tree, branch);

	localGroup->show();
	curveEditor->setEnabled(true);

	if (activeCurve)
		activeCurve->select();

	if (tm_is_terminal_branch(tree, branch))
		resolution->setEnabled(false);
	else
		resolution->setEnabled(true);
}

void PropertyBox::fillCurveButtons(tm_tree tree, int branch)
{
	int size;
	tm_vec3 *l;
	tm_get_radius_curve(tree, branch, &l, &size);
	radiusCB->getCurveButton()->setControls(l, size);
}

void PropertyBox::bind(SceneEditor *sceneEditor, CurveEditor *curveEditor)
{
	this->curveEditor = curveEditor;
	this->sceneEditor = sceneEditor;

	connect(sceneEditor, SIGNAL(selectionChanged(tm_tree, int)), this,
			SLOT(fill(tm_tree, int)));
	connect(resolution, SIGNAL(valueChanged(int)), sceneEditor,
			SLOT(changeResolution(int)));
	connect(sections, SIGNAL(valueChanged(int)), sceneEditor,
			SLOT(changeSections(int)));
	connect(radius, SIGNAL(valueChanged(double)), sceneEditor,
			SLOT(changeRadius(double)));
	connect(branches, SIGNAL(valueChanged(double)), sceneEditor,
			SLOT(changeBranchDensity(double)));

	bindCurveEditor();
}

void PropertyBox::bindCurveEditor()
{
	connect(curveEditor, SIGNAL(curveChanged(vector<tm_vec3>, QString)),
			this, SLOT(setCurve(vector<tm_vec3>, QString)));

	connect(radiusCB, SIGNAL(selected(CurveButtonWidget *)), this,
			SLOT(toggleCurve(CurveButtonWidget *)));
}