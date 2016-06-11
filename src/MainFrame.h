#pragma once
#include <QtWidgets/QtWidgets>
#include <string>
#include "SimpleModel.h"
#include "MyCanvas.h"
#include "PotentialFieldFrame.h"
#include "PathAnimator.h"
#include "VisitedFrame.h"
#include "FindPathRunner.h"

namespace IMMP{
class MainFrame : public QWidget{
	Q_OBJECT
public:
	MainFrame(IMMP::SimpleModel& s);
	~MainFrame(void);
	void setUIState();
	void loadDATFiles();
	void addComboBoxMenu(QComboBox*, const QString&);

signals:
    void ModelUpdated();

private slots:
	void loadRobot(const QString&);
	void loadObstacle(const QString&);
	void reset();
	void findPath();
	void showPotentialField();
	void showCSpace();
	void showPath();
	void showAnimation();
	void smoothPath();
	void quit();

private:
    void updateControlPointWeightComboBox();

private:
	QHBoxLayout *panel;
	QVBoxLayout *rightPanel;
	QPushButton *resetButton;
	QPushButton *findPathButton;
	QPushButton *showPotentialFieldButton;
	QPushButton *showCSpaceButton;
	QPushButton *showPathButton;
	QPushButton *showAnimationButton;
	QPushButton *smoothButton;
	QPushButton *quitButton;
	QComboBox *obstacleComboBox;
	QComboBox *robotComboBox;
    QComboBox *controlPointComboBox;
    QLineEdit *controlPointWeightLineEdit;
	QCheckBox *isCspaceComboBox;
	QLabel *authorLabel;
	IMMP::MyCanvas* canvas;
	SimpleModel& model;
	PotentialFieldFrame* potentialFieldFrame;
	VisitedFrame* visitedFrame;
	
	PathAnimator pa;
	FindPathRunner fpr;
};

};
