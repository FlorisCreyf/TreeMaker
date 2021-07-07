/* Plant Generator
 * Copyright (C) 2020  Floris Creyf
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

#ifndef PATTERN_EDITOR_H
#define PATTERN_EDITOR_H

#include "editor.h"
#include "widgets.h"
#include "curve_editor.h"
#include "../commands/generate.h"
#include <string>

class PatternEditor : public QWidget {
	Q_OBJECT

	Editor *editor;
	KeyMap *keymap;
	SharedResources *shared;
	Generate *generate;

	ComboBox *nodeValue;
	QPushButton *childButton;
	QPushButton *siblingButton;
	QPushButton *removeButton;

	enum {RootLength, RootFork, RootForkAngle, RootThreshold, RootNoise,
		DRSize};
	enum {Seed, IRSize};
	SpinBox *irv[IRSize];
	DoubleSpinBox *drv[DRSize];
	enum {StemDensity, StemDistance, Length, Scale, AngleVariation, Noise,
		RadiusThreshold, Fork, ForkAngle, LeafDensity, LeafDistance,
		LeafRotation, MinUp, MaxUp, LocalUp, GlobalUp, MinForward,
		MaxForward, Pull, ScaleX, ScaleY, ScaleZ, DSize};
	enum {LeavesPerNode, ISize};
	DoubleSpinBox *dv[DSize];
	SpinBox *iv[ISize];

	void createInterface();
	void createStemLeafFields();
	void createStemGroup(QBoxLayout *);
	void createLeafGroup(QBoxLayout *);
	void createNodeGroup(QBoxLayout *);
	void createRootGroup(QBoxLayout *);
	void removeCurrent();
	void blockSignals(bool);
	void setEnabled(bool);
	void createCommand();
	void setFields(const pg::ParameterTree &, std::string);
	void setStemData(pg::StemData);
	void setLeafData(pg::LeafData);
	pg::StemData getRootData(pg::StemData);
	pg::StemData getStemData(pg::StemData);
	pg::LeafData getLeafData(pg::LeafData);

	CurveEditor *curveEditor;
	ComboBox *curveDegree;
	ComboBox *curveType;
	ComboBox *curveNode;

	void createCurveGroup(QBoxLayout *);
	void setCurveFields();
	void selectCurve();
	void enableCurve(bool);
	void updateCurve(pg::Spline);
	void updateParameterTree(pg::Spline);

public:
	PatternEditor(SharedResources *shared, KeyMap *keymap,
		Editor *editor, QWidget *parent);
	QSize sizeHint() const;
	void change();
	void select();
	void addChildNode();
	void addSiblingNode();
	void removeNode();

public slots:
	void setFields();
};

#endif