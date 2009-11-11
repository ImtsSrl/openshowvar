/***************************************************************************
 *   Copyright (C) 2007 by Massimiliano Fago                               *
 *   massimiliano.fago@gmail.com                                           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/*!	\file
 *	\class RobotVarEdit
 *	\author Massimiliano Fago
 *	\version 1.0
 *	\date 2007
 *	\brief Edit variabile
 *
 */

#include "robotvaredit.h"

///\todo Inserire un nuovo costruttore che riceve una KUKAVAR
//TODO Inserire un nuovo costruttore che riceve una KUKAVAR

RobotVarEdit::RobotVarEdit(const QByteArray *variabile, const QByteArray *varname, const QHostAddress varip, QWidget *parent)
	: QDialog(parent)
{
	this->varip = varip;
	
	setWindowTitle(tr("Variable value edit"));
	
	robotvar = new KukaVar(varname, variabile);
	
	QVBoxLayout *topLayout = new QVBoxLayout;
	QGridLayout *leftLayout = new QGridLayout;
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	QSignalMapper* mapper[40];
	
	switch(robotvar->getVarType()){
		case STRUCTURE:
		{
			//top labels
			QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
			titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			
			QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getStructureName()));
			structureLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			topLayout->addWidget(titleLabel);
			topLayout->addWidget(structureLabel);
			topLayout->addStretch();
			
			for (int i=0;i<robotvar->getElementsNumber();i++)
			{
				int tipodato;
				robotvar->getStructureValue(i,tipodato);
				
				widget[i] = new QLabel(robotvar->getStructureMember(i));
				switch(tipodato){
					case INT:
						widget[i+robotvar->getElementsNumber()] = new QSpinBox();
						((QSpinBox*)widget[i+robotvar->getElementsNumber()])->setRange(-9999,9999);
						((QSpinBox*)widget[i+robotvar->getElementsNumber()])->setValue(robotvar->getStructureValue(i,tipodato).toInt());
						
						mapper[i] = new QSignalMapper(this);
						connect(widget[i+robotvar->getElementsNumber()], SIGNAL(valueChanged(const QString&)),mapper[i], SLOT(map()));
						mapper[i]->setMapping(widget[i+robotvar->getElementsNumber()], i);
						connect(mapper[i], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
						
						break;
					case REAL:
						widget[i+robotvar->getElementsNumber()] = new QDoubleSpinBox();
						((QDoubleSpinBox*)widget[i+robotvar->getElementsNumber()])->setDecimals(6);
						((QDoubleSpinBox*)widget[i+robotvar->getElementsNumber()])->setRange(-9999,9999);
						((QDoubleSpinBox*)widget[i+robotvar->getElementsNumber()])->setValue(robotvar->getStructureValue(i,tipodato).toDouble());
						
						mapper[i] = new QSignalMapper(this);
						connect(widget[i+robotvar->getElementsNumber()], SIGNAL(valueChanged(const QString&)),mapper[i], SLOT(map()));
						mapper[i]->setMapping(widget[i+robotvar->getElementsNumber()], i);
						connect(mapper[i], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
						
						break;
					case BOOL:
						widget[i+robotvar->getElementsNumber()] = new QComboBox();
						((QComboBox*)widget[i+robotvar->getElementsNumber()])->addItem("TRUE");
						((QComboBox*)widget[i+robotvar->getElementsNumber()])->addItem("FALSE");
						((QComboBox*)widget[i+robotvar->getElementsNumber()])->setCurrentIndex(((QComboBox*)widget[i+robotvar->getElementsNumber()])->findText(robotvar->getStructureValue(i,tipodato)));
						
						mapper[i] = new QSignalMapper(this);
						connect(widget[i+robotvar->getElementsNumber()], SIGNAL(currentIndexChanged(const QString&)),mapper[i], SLOT(map()));
						mapper[i]->setMapping(widget[i+robotvar->getElementsNumber()], i);
						connect(mapper[i], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
						
						break;
					case CHAR:
					{
						widget[i+robotvar->getElementsNumber()] = new QLineEdit(robotvar->getStructureValue(i,tipodato));
			
						QRegExp regExp("[A-Za-z0-9]{1}");
						((QLineEdit*)widget[i+robotvar->getElementsNumber()])->setValidator(new QRegExpValidator(regExp, this));
						
						mapper[i] = new QSignalMapper(this);
						connect(widget[i+robotvar->getElementsNumber()], SIGNAL(textChanged(const QString&)),mapper[i], SLOT(map()));
						mapper[i]->setMapping(widget[i+robotvar->getElementsNumber()], i);
						connect(mapper[i], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
						
						break;
					}
					default:
						widget[i+robotvar->getElementsNumber()] = new QLineEdit(robotvar->getStructureValue(i,tipodato));
						break;
				}
				
				//QRegExp regExp("[A-Za-z0-9]{1,30}");
				//namedLineEdit[i]->setValidator(new QRegExpValidator(regExp, this));
				//connect(widget[i+robotvar->getElementsNumber()], SIGNAL(textChanged(const QString &)), this, SLOT(on_namedLineEdit_textChanged(const QString &)));
			
				leftLayout->addWidget(widget[i],i,0);
				leftLayout->addWidget(widget[i+robotvar->getElementsNumber()],i,1);
			}
			break;
		}
		case INT:
		{
			//INT
			//top labels
			QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
			titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getVarTypeName()));
			structureLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			topLayout->addWidget(titleLabel);
			topLayout->addWidget(structureLabel);
			topLayout->addStretch();
			
			widget[0] = new QLabel(robotvar->getVarName());
			widget[1] = new QSpinBox();
			((QSpinBox*)widget[1])->setRange(-(9999999),(9999999));
			((QSpinBox*)widget[1])->setValue(robotvar->getValue().toInt());
			
			leftLayout->addWidget(widget[0],0,0);
			leftLayout->addWidget(widget[1],0,1);
			
			mapper[0] = new QSignalMapper(this);
			connect(widget[1], SIGNAL(valueChanged(const QString&)),mapper[0], SLOT(map()));
			mapper[0]->setMapping(widget[1], 0);
			connect(mapper[0], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
			
			break;
		}
		case REAL:
		{
			//REAL
			//top labels
			QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
			titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getVarTypeName()));
			structureLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			topLayout->addWidget(titleLabel);
			topLayout->addWidget(structureLabel);
			topLayout->addStretch();
			
			widget[0] = new QLabel(robotvar->getVarName());
			widget[1] = new QDoubleSpinBox();
			((QDoubleSpinBox*)widget[1])->setDecimals(4);
			((QDoubleSpinBox*)widget[1])->setRange(-9999,9999);
			((QDoubleSpinBox*)widget[1])->setValue(robotvar->getValue().toDouble());
		
			leftLayout->addWidget(widget[0],0,0);
			leftLayout->addWidget(widget[1],0,1);
			
			mapper[0] = new QSignalMapper(this);
			connect(widget[1], SIGNAL(valueChanged(const QString&)),mapper[0], SLOT(map()));
			mapper[0]->setMapping(widget[1], 0);
			connect(mapper[0], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
			
			break;
		}
		case BOOL:
		{
			//BOOL
			//top labels
			QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
			titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getVarTypeName()));
			structureLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			topLayout->addWidget(titleLabel);
			topLayout->addWidget(structureLabel);
			topLayout->addStretch();
			
			widget[0] = new QLabel(robotvar->getVarName());
			widget[1] = new QComboBox();
			((QComboBox*)widget[1])->addItem("TRUE");
			((QComboBox*)widget[1])->addItem("FALSE");
			((QComboBox*)widget[1])->setCurrentIndex(((QComboBox*)widget[1])->findText(robotvar->getValue()));
			
			leftLayout->addWidget(widget[0],0,0);
			leftLayout->addWidget(widget[1],0,1);
			
			mapper[0] = new QSignalMapper(this);
			connect(widget[1], SIGNAL(currentIndexChanged(const QString&)),mapper[0], SLOT(map()));
			mapper[0]->setMapping(widget[1], 0);
			connect(mapper[0], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
			
			break;
		}
		case CHAR:
		{
			//CHAR
			//top labels
			QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
			titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getVarTypeName()));
			structureLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			topLayout->addWidget(titleLabel);
			topLayout->addWidget(structureLabel);
			topLayout->addStretch();
			
			widget[0] = new QLabel(robotvar->getVarName());
			widget[1] = new QLineEdit(robotvar->getValue());
			
			QRegExp regExp("[A-Za-z0-9]{1}");
			((QLineEdit*)widget[1])->setValidator(new QRegExpValidator(regExp, this));
			connect(widget[1], SIGNAL(textChanged(const QString &)), this, SLOT(on_namedLineEdit_textChanged(const QString &)));
		
			leftLayout->addWidget(widget[0],0,0);
			leftLayout->addWidget(widget[1],0,1);
			
			mapper[0] = new QSignalMapper(this);
			connect(widget[1], SIGNAL(valueChanged(const QString&)),mapper[0], SLOT(map()));
			mapper[0]->setMapping(widget[1], 0);
			connect(mapper[0], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
			
			break;
		}
	}
	
	//bottom commands
	QPushButton *okButton = new QPushButton(tr("OK"));
	QPushButton *abortButton = new QPushButton(tr("Abort"));
	bottomLayout->addWidget(okButton);
	bottomLayout->addWidget(abortButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
	
	//connection slots and signals
	connect(okButton, SIGNAL(clicked()), this, SLOT(on_okButton()));
	connect(abortButton, SIGNAL(clicked()), this, SLOT(on_abortButton()));
}

RobotVarEdit::~RobotVarEdit()
{
	qDebug() << "Chiamato distruttore";
}

void RobotVarEdit::on_okButton()
{
	switch(robotvar->getVarType()){
		case STRUCTURE:
		{
			//set new value for robotvar
			for (int i=0;i<robotvar->getElementsNumber();i++){
				//qDebug() << widget[i+robotvar->getElementsNumber()];
				if (QSpinBox* spinBox = dynamic_cast<QSpinBox*>(widget[i+robotvar->getElementsNumber()]))
				{
					robotvar->setFieldValue(((QSpinBox*)widget[i+robotvar->getElementsNumber()])->text().toAscii(),i);
				}
				else if (QDoubleSpinBox* doublespinBox = dynamic_cast<QDoubleSpinBox*>(widget[i+robotvar->getElementsNumber()]))
				{
					robotvar->setFieldValue(((QDoubleSpinBox*)widget[i+robotvar->getElementsNumber()])->text().replace(",",".").toAscii(),i);
				}
				else if (QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget[i+robotvar->getElementsNumber()]))
				{
					robotvar->setFieldValue(((QLineEdit*)widget[i+robotvar->getElementsNumber()])->text().toAscii(),i);
				}
				else if (QComboBox* comboBox = dynamic_cast<QComboBox*>(widget[i+robotvar->getElementsNumber()]))
				{
					robotvar->setFieldValue(((QComboBox*)widget[i+robotvar->getElementsNumber()])->currentText().toAscii(),i);
				}
			}
			//send new value to robot
			emit writevalue(robotvar->getVarName(),QByteArray(robotvar->createStructure()), varip);
			break;
		}
		case INT:
		{
			//INT
			robotvar->setValue(((QSpinBox*)widget[1])->text().toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
		case REAL:
		{
			//REAL
			robotvar->setValue(((QDoubleSpinBox*)widget[1])->text().replace(",",".").toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
		case BOOL:
		{
			//BOOL
			robotvar->setValue(((QComboBox*)widget[1])->currentText().toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
		case CHAR:
		{
			//CHAR
			robotvar->setValue(((QLineEdit*)widget[1])->text().toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
		default:
		{
			qDebug() << "ANOMALIA!!";
			break;
		}
	}
	on_Accept();
	//FIXME this command close everytime the window. I want rally this?
	//accept();
}

void RobotVarEdit::on_abortButton()
{
	reject();
}

void RobotVarEdit::on_namedLineEdit_textChanged(const QString &text)
{
	//okButton->setEnabled(lineEdit->hasAcceptableInput());
}

/*!	\brief Modifica label descrizione
 *	
 *	Modifica il contenuto e il colore della label di descrizione
 *	Nella label dopo la modifica, viene messo tra parentesi il
 *	vecchio valore
 *
 *	\param index Indice della label da modificare
 */

void RobotVarEdit::on_Changed(int index)
{
	int datatype;
	
	QPalette p = widget[index]->palette();
	p.setColor(QPalette::WindowText, Qt::red);
	widget[index]->setPalette(p);
	widget[index]->setAutoFillBackground(true);
	
	if (robotvar->getVarType()==STRUCTURE)
		((QLabel*)widget[index])->setText(robotvar->getStructureMember(index) + " (" + robotvar->getStructureValue(index,datatype) + ")");
	else
		((QLabel*)widget[index])->setText(robotvar->getVarName() + " (" + robotvar->getValue() + ")");
}

/*!	\brief Ripristino label
 *	
 *	Ripristina il contenuto e il colore della label di descrizione
 *
 */

void RobotVarEdit::on_Accept()
{
	for(int i=0;i<robotvar->getElementsNumber();i++){
		QPalette p = widget[i]->palette();
		p.setColor(QPalette::WindowText, Qt::black);
		widget[i]->setPalette(p);
		widget[i]->setAutoFillBackground(true);
		
		if (robotvar->getVarType()==STRUCTURE)
			((QLabel*)widget[i])->setText(robotvar->getStructureMember(i));
		else
			((QLabel*)widget[i])->setText(robotvar->getVarName());
	}
}
