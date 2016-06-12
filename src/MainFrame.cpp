#include "MainFrame.h"
#include "SimpleModel.h"
#include "SceneReader.h"
#include "BFS.h"
#include "PathAnimator.h"
#include "PathSmoother.h"
#include "Util.h"
#include "FindPathRunner.h"
#include "CollisionDetector.h"
#include <QtCore/QDateTime>

using namespace IMMP;

MainFrame::MainFrame(IMMP::SimpleModel& s) : model(s)
{
	rightPanel = new QVBoxLayout;
	panel = new QHBoxLayout;
	canvas = new IMMP::MyCanvas(model.getRobots(), model.getObstacles());
	canvas->setMinimumSize(400, 400);
	resetButton = new QPushButton(tr("Reset"));
	resetButton->setFixedWidth(120);
	findPathButton = new QPushButton(tr("FindPath"));
	findPathButton->setFixedWidth(120);
	showPotentialFieldButton = new QPushButton(tr("Show PotentialField"));
	showPotentialFieldButton->setFixedWidth(120);
	showCSpaceButton = new QPushButton(tr("Show CSpace/Visited"));
	showCSpaceButton->setFixedWidth(120);
	showPathButton = new QPushButton(tr("ShowPath"));
	showPathButton->setFixedWidth(120);
	showAnimationButton = new QPushButton(tr("ShowAnimation"));
    showAnimationButton->setFixedWidth(120);
	smoothButton = new QPushButton(tr("Smooth"));
	smoothButton->setFixedWidth(120);
	quitButton = new QPushButton(tr("Quit"));
	quitButton->setFixedWidth(120);
	authorLabel = new QLabel(tr("By Pandia 2013"));
	obstacleComboBox = new QComboBox;
	obstacleComboBox->setFixedWidth(120);
	robotComboBox = new QComboBox;
	robotComboBox->setFixedWidth(120);
    controlPointComboBox = new QComboBox;
    controlPointComboBox->setFixedWidth(120);

    controlPointWeightLineEdit = new QLineEdit;
    controlPointWeightLineEdit->setFixedWidth(120);

	obstacleComboBox->addItem(tr("Select Obstacles"));
    /*obstacleComboBox->addItem(tr("map00.dat"));
	obstacleComboBox->addItem(tr("map01.dat"));
	obstacleComboBox->addItem(tr("map02.dat"));
	obstacleComboBox->addItem(tr("map03.dat"));
	obstacleComboBox->addItem(tr("map04.dat"));
	obstacleComboBox->addItem(tr("map05.dat"));
	obstacleComboBox->addItem(tr("map06.dat"));
	obstacleComboBox->addItem(tr("map07.dat"));*/

	robotComboBox->addItem(tr("Select Robots"));
	/*robotComboBox->addItem(tr("robot00.dat"));
	robotComboBox->addItem(tr("robot01.dat"));
	robotComboBox->addItem(tr("robot02.dat"));
	robotComboBox->addItem(tr("robot03.dat"));
	robotComboBox->addItem(tr("robot04.dat"));
	robotComboBox->addItem(tr("robot05.dat"));
	robotComboBox->addItem(tr("robot06.dat"));
	robotComboBox->addItem(tr("robot07.dat"));*/

    controlPointComboBox->addItem(tr("Select Control Point Weight"));
    controlPointWeightLineEdit->setValidator(new QDoubleValidator(0, Constant::CONTROLPOINT_WEIGHT_MAX, 2, this));

	loadDATFiles();

	isCspaceComboBox = new QCheckBox(tr("Use Cspace"));

	QObject::connect(findPathButton, SIGNAL(clicked()), this, SLOT(findPath()));
	QObject::connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect(showPotentialFieldButton, SIGNAL(clicked()), this, SLOT(showPotentialField()));
	QObject::connect(showCSpaceButton, SIGNAL(clicked()), this, SLOT(showCSpace()));
	QObject::connect(showPathButton, SIGNAL(clicked()), this, SLOT(showPath()));
	QObject::connect(showAnimationButton, SIGNAL(clicked()), this, SLOT(showAnimation()));
	QObject::connect(smoothButton, SIGNAL(clicked()), this, SLOT(smoothPath()));
	QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
	QObject::connect(robotComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(loadRobot(const QString&)));
	QObject::connect(obstacleComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(loadObstacle(const QString&)));
    QObject::connect(controlPointComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                     this, [=](int index){
                        bool isNoRobot = model.getRobots().empty();

                        if(index == 0 || isNoRobot){
                            controlPointWeightLineEdit->setText("");
                            canvas->setSelectedControlPoint(SIZE_MAX);
                            Q_EMIT ModelUpdated();
                            return;
                        }

                        const Robot& robot = model.getRobots().at(0);
                        controlPointWeightLineEdit->setText(QString::number(robot.getControlPointWeight((size_t)index - 1)));
                        canvas->setSelectedControlPoint((size_t)index - 1);
                        Q_EMIT ModelUpdated();
                    });

    QObject::connect(controlPointWeightLineEdit, &QLineEdit::textChanged, this, [=]
    {
        int currentIndex = controlPointComboBox->currentIndex();

        if(currentIndex == 0 || model.getRobots().empty()){
            return;
        }

        Robot& robot = model.getRobots().at(0);
        robot.setControlPointWeight(currentIndex - 1, controlPointWeightLineEdit->text().toDouble());

    });
    QObject::connect(this, &MainFrame::ModelUpdated, this, [=]{ canvas->update(); });

	rightPanel->addWidget(robotComboBox);
	rightPanel->addWidget(obstacleComboBox);
    rightPanel->addWidget(controlPointComboBox);
    rightPanel->addWidget(new QLabel(tr("CP Weight: [0,10]")));
    rightPanel->addWidget(controlPointWeightLineEdit);
	rightPanel->addWidget(resetButton);
	rightPanel->addWidget(findPathButton);
	rightPanel->addWidget(isCspaceComboBox);
	rightPanel->addWidget(showPotentialFieldButton);
	rightPanel->addWidget(showCSpaceButton);
	rightPanel->addWidget(showPathButton);
	rightPanel->addWidget(showAnimationButton);
	rightPanel->addWidget(smoothButton);
	rightPanel->addWidget(quitButton);
	rightPanel->addStretch();
	rightPanel->addWidget(authorLabel);
	panel->addWidget(canvas);
	panel->addLayout(rightPanel);
	this->setLayout(panel);
	potentialFieldFrame = NULL;
	visitedFrame = NULL;
	fpr.setCanvas(canvas);
	fpr.setModel(&model);
	fpr.setFrame(this);
	setUIState();
}


MainFrame::~MainFrame(void)
{
	delete findPathButton;
	delete isCspaceComboBox;
	delete showPotentialFieldButton;
	delete showCSpaceButton;
	delete showPathButton;
	delete showAnimationButton;
	delete smoothButton;
	delete quitButton;
	delete resetButton;
	delete obstacleComboBox;
	delete robotComboBox;
    delete controlPointComboBox;
    delete controlPointWeightLineEdit;
	delete canvas;
	delete rightPanel;

	delete potentialFieldFrame;
	delete visitedFrame;

}

void MainFrame::loadDATFiles(){
	addComboBoxMenu(robotComboBox, QString(tr("DAT/Robots/")));
	addComboBoxMenu(obstacleComboBox, QString(tr("DAT/Obstacles/")));
}

void MainFrame::addComboBoxMenu(QComboBox* comboBox, const QString& path){
	QDir dir(path);
	QStringList filter;
	filter << "*.dat" << "*.DAT";
	QStringList fileList = dir.entryList(filter);

	for(QString file : fileList){
		comboBox->addItem(file);
	}
}

void MainFrame::reset(){
	if(pa.isRunning()){
		pa.stop();
		pa.wait();
	}
	if(fpr.isRunning()){
		fpr.terminate();
		fpr.wait();
	}
	model.reset();
	canvas->setDrawPath(false);
	canvas->setShowAnimation(false);
	canvas->setMessage("");
	loadRobot(robotComboBox->currentText());
	loadObstacle(obstacleComboBox->currentText());
	if(visitedFrame != NULL){
		visitedFrame->hide();
		delete visitedFrame;
		visitedFrame = NULL;
	}

	if(potentialFieldFrame != NULL){
		potentialFieldFrame->hide();
		delete potentialFieldFrame;
		potentialFieldFrame = NULL;
	}

	setUIState();
}

void MainFrame::loadRobot(const QString& str){
	canvas->setDrawPath(false);
	model.clearPath();
	QString path(tr("DAT/Robots/"));
	path.append(tr(str.toStdString().c_str()));
	model.setRobots(IMMP::SceneReader::openRobotFile(QDir::toNativeSeparators(path).toStdString()));
    updateControlPointWeightComboBox();
    Q_EMIT ModelUpdated();
}

void MainFrame::loadObstacle(const QString& str){
	canvas->setDrawPath(false);
	model.clearPath();
	QString path(tr("DAT/Obstacles/"));
	path.append(tr(str.toStdString().c_str()));
	model.setObstacles(IMMP::SceneReader::openObstacleFile(QDir::toNativeSeparators(path).toStdString()));
    Q_EMIT ModelUpdated();
}

void MainFrame::updateControlPointWeightComboBox()
{
    if (model.getRobots().empty())
    {
        return;
    }

    const Robot& robot = model.getRobots().at(0);

    controlPointComboBox->clear();
    controlPointComboBox->addItem(tr("Select Control Point Weight"));
    for (size_t i = 0; i < robot.NControlPoints(); ++i)
    {
        controlPointComboBox->addItem("CP " + QString::number(i + 1));
    }
}

void MainFrame::findPath(){
	canvas->setDrawPath(false);
	canvas->update();
	if(pa.isRunning()){
		pa.stop();
		pa.wait();
	}
	if(fpr.isRunning()){
		fpr.terminate();
		fpr.wait();
	}
	model.reset();
	canvas->setDrawPath(false);
	canvas->setShowAnimation(false);
	canvas->setMessage("");
	//loadRobot(robotComboBox->currentText());
	//loadObstacle(obstacleComboBox->currentText());
	if(visitedFrame != NULL){
		visitedFrame->hide();
		delete visitedFrame;
		visitedFrame = NULL;
	}

	if(potentialFieldFrame != NULL){
		potentialFieldFrame->hide();
		delete potentialFieldFrame;
		potentialFieldFrame = NULL;
	}
	/*QTime time;
	time.start();
	model.findPath(isCspaceComboBox->isChecked());
	int delta = time.elapsed();
	if(!model.getPath().empty()){
		QString msg(tr("Path Found in "));
		msg.append(tr(Util<int>::toString(delta).c_str()));
		msg.append(tr(" milliseconds."));
		canvas->setMessage(msg);
		canvas->setDrawPath(true);
		canvas->setPath(&model.getPath());
	}
	else{
		canvas->setMessage(QString(tr("Path Not Found.")));
	}*/

	if(fpr.isRunning()){
		fpr.terminate();
		fpr.wait();
		//fpr.setModel(NULL);
		//fpr.setCanvas(NULL);
		canvas->update();
	}

	const Robot& robot = model.getRobots().at(0);

	if(CollisionDetector::isRobotCollide(robot, robot.getInitialConfiguration(), model.getObstacles()) ||
		CollisionDetector::isRobotCollide(robot, robot.getGoalConfiguration(), model.getObstacles())){
		canvas->setMessage(tr("Robot is collided with obstacles"));
		canvas->update();
		return;
	}

	//fpr.setCanvas(canvas);
	//fpr.setModel(&model);
	fpr.setUseCSpace(isCspaceComboBox->isChecked());
	fpr.start();
	model.setState(SimpleModel::STATE_FINDING);
	setUIState();
	canvas->update();
}

void MainFrame::showPotentialField(){
	if(potentialFieldFrame != NULL){
		delete potentialFieldFrame;
	}

	potentialFieldFrame = new PotentialFieldFrame(model);
	potentialFieldFrame->show();
}

void MainFrame::showCSpace(){
	if(visitedFrame != NULL){
		delete visitedFrame;
	}

	visitedFrame = new VisitedFrame(model);
	visitedFrame->show();
	//for(int i = 0; i < Constant::ANGLESPACE; i++){
	//	VisitedCanvas *vc = new VisitedCanvas(i, model);
	//	vc->show();
	//}

}

void MainFrame::showPath(){
	canvas->setDrawPath(!canvas->isDrawPath());
	canvas->setShowAnimation(false);
	canvas->update();
}

void MainFrame::showAnimation(){

	if(pa.isRunning()){
		pa.stop();
		pa.wait();
		pa.setPath(NULL);
		pa.setCanvas(NULL);
		canvas->update();
	}
	canvas->setDrawPath(false);
	canvas->setShowAnimation(true);
	pa.setPath(&model.getPath());
	pa.setCanvas(canvas);
	pa.start();
	canvas->update();
}

void MainFrame::smoothPath(){
	PathSmoother ps(model);
	QTime time;
	time.start();
	model.setPath(ps.smooth(model.getPath()));
	int delta = time.elapsed();
	QString msg(tr("Smooth took "));
	msg.append(tr(Util<int>::toString(delta).c_str()));
	msg.append(tr(" milliseconds."));
	canvas->setMessage(msg);
	canvas->update();
}

void MainFrame::quit(){
	exit(0);
}

void MainFrame::setUIState(){
	if(model.getState() == SimpleModel::STATE_FOUND){
		robotComboBox->setEnabled(false);
		obstacleComboBox->setEnabled(false);
        controlPointComboBox->setEnabled(false);
        controlPointWeightLineEdit->setEnabled(false);
		findPathButton->setEnabled(false);
		isCspaceComboBox->setEnabled(false);
		showPotentialFieldButton->setEnabled(true);
		showCSpaceButton->setEnabled(true);
		showPathButton->setEnabled(true);
		showAnimationButton->setEnabled(true);
		smoothButton->setEnabled(true);
		canvas->setEnabled(false);
		//resetButton->setEnabled(true);
	}
	else if(model.getState() == SimpleModel::STATE_NOTFOUND){
		robotComboBox->setEnabled(true);
		obstacleComboBox->setEnabled(true);
        controlPointComboBox->setEnabled(true);
        controlPointWeightLineEdit->setEnabled(true);
		findPathButton->setEnabled(true);
		isCspaceComboBox->setEnabled(true);
		showPotentialFieldButton->setEnabled(true);
		showCSpaceButton->setEnabled(true);
		showPathButton->setEnabled(false);
		showAnimationButton->setEnabled(false);
		smoothButton->setEnabled(false);
		canvas->setEnabled(true);
		//resetButton->setEnabled(true);
	}
	else if(model.getState() == SimpleModel::STATE_INIT){
		robotComboBox->setEnabled(true);
		obstacleComboBox->setEnabled(true);
        controlPointComboBox->setEnabled(true);
        controlPointWeightLineEdit->setEnabled(true);
		findPathButton->setEnabled(true);
		isCspaceComboBox->setEnabled(true);
		showPotentialFieldButton->setEnabled(false);
		showCSpaceButton->setEnabled(false);
		showPathButton->setEnabled(false);
		showAnimationButton->setEnabled(false);
		smoothButton->setEnabled(false);
		canvas->setEnabled(true);
		//resetButton->setEnabled(true);
	}
	else if(model.getState() == SimpleModel::STATE_FINDING){
		robotComboBox->setEnabled(false);
		obstacleComboBox->setEnabled(false);
        controlPointComboBox->setEnabled(false);
        controlPointWeightLineEdit->setEnabled(false);
		findPathButton->setEnabled(false);
		isCspaceComboBox->setEnabled(false);
		showPotentialFieldButton->setEnabled(false);
		showCSpaceButton->setEnabled(false);
		showPathButton->setEnabled(false);
		showAnimationButton->setEnabled(false);
		smoothButton->setEnabled(false);
		canvas->setEnabled(false);
		//resetButton->setEnabled(false);
	}
	else{
		robotComboBox->setEnabled(false);
		obstacleComboBox->setEnabled(false);
        controlPointComboBox->setEnabled(false);
        controlPointWeightLineEdit->setEnabled(false);
		findPathButton->setEnabled(false);
		isCspaceComboBox->setEnabled(false);
		showPotentialFieldButton->setEnabled(false);
		showCSpaceButton->setEnabled(false);
		showPathButton->setEnabled(false);
		showAnimationButton->setEnabled(false);
		smoothButton->setEnabled(false);
		canvas->setEnabled(false);
		resetButton->setEnabled(false);
	}
	update();
	/*else{
		robotComboBox->setEnabled(true);
		obstacleComboBox->setEnabled(true);
		findPathButton->setEnabled(true);
		isCspaceComboBox->setEnabled(true);
		if(model.getBFS() != NULL){
			showPotentialFieldButton->setEnabled(true);
			showCSpaceButton->setEnabled(true);
		}
		else{
			showPotentialFieldButton->setEnabled(false);
			showCSpaceButton->setEnabled(false);
		}
		showPathButton->setEnabled(false);
		showAnimationButton->setEnabled(false);
		smoothButton->setEnabled(false);
		canvas->setEnabled(true);
	}*/
}
