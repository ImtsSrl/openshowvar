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
        //QSignalMapper* mapper[40];

/*
        QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
        titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getStructureName()));
        QLabel *structureLabel = new QLabel(tr("Variable type: %1").arg((QString)robotvar->getVarTypeName()));
        structureLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        topLayout->addWidget(titleLabel);
        topLayout->addWidget(structureLabel);
        topLayout->addStretch();

        
        //QLabel *titleLabel = new QLabel(tr("Variable edit: %1").arg(varname->data()));
        //titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        
        for(int index=0;index<robotvar->getElementsNumber();index++)
        {
            

            //qDebug() << "Tipo dato: " << robotvar->getVarType();

            switch(robotvar->getVarType()){
            case STRUCTURE:
                {
                    break;
                }
            case INT:
                {
                    addInt(1,robotvar->getVarName(),robotvar->getValue().toInt());
                    break;
                }
            case REAL:
                {
                    addReal(1,robotvar->getVarName(),robotvar->getValue().toDouble());
                    break;
                }
            case BOOL:
                {
                    addBool(1,robotvar->getVarName(),robotvar->getValue());
                    break;
                }
            case CHAR:
                {
                    addChar(1,robotvar->getVarName(),robotvar->getValue());
                    break;
                }
            }

            leftLayout->addWidget(widget[0],0,0);
            leftLayout->addWidget(widget[1],0,1);
            addMap(index+1,0,robotvar->getVarType());

        }
*/

        switch(robotvar->getVarType()){
                case KukaVar::STRUCTURE:
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
                                        case KukaVar::INT:
                                                addInt(i+robotvar->getElementsNumber(),robotvar->getStructureMember(i), robotvar->getStructureValue(i,tipodato).toInt());
                                                addMap(i+robotvar->getElementsNumber(),i,tipodato);

                                                break;
                                        case KukaVar::REAL:
                                                addInt(i+robotvar->getElementsNumber(),robotvar->getStructureMember(i), robotvar->getStructureValue(i,tipodato).toDouble());
                                                addMap(i+robotvar->getElementsNumber(),i,tipodato);

                                                break;
                                        case KukaVar::BOOL:
                                                addBool(i+robotvar->getElementsNumber(),robotvar->getStructureMember(i), robotvar->getStructureValue(i,tipodato));
                                                addMap(i+robotvar->getElementsNumber(),i,tipodato);

                                                break;
                                        case KukaVar::CHAR:
                                        {
                                                addChar(i+robotvar->getElementsNumber(),robotvar->getStructureMember(i), robotvar->getStructureValue(i,tipodato));
                                                addMap(i+robotvar->getElementsNumber(),i,tipodato);
                                                break;
                                        }
                                        default:
                                                widget[i+robotvar->getElementsNumber()] = new QLineEdit(robotvar->getStructureValue(i,tipodato));
                                                break;
                                }

                                leftLayout->addWidget(widget[i],i,0);
                                leftLayout->addWidget(widget[i+robotvar->getElementsNumber()],i,1);
                        }
                        break;
                }
                case KukaVar::INT:
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

                        addInt(1,robotvar->getVarName(),robotvar->getValue().toInt());

                        leftLayout->addWidget(widget[0],0,0);
                        leftLayout->addWidget(widget[1],0,1);

                        addMap(1,0,robotvar->getVarType());

                        break;
                }
                case KukaVar::REAL:
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

                        addReal(1,robotvar->getVarName(),robotvar->getValue().toDouble());

                        leftLayout->addWidget(widget[0],0,0);
                        leftLayout->addWidget(widget[1],0,1);

                        addMap(1,0,robotvar->getVarType());

                        break;
                }
                case KukaVar::BOOL:
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

                        addBool(1,robotvar->getVarName(),robotvar->getValue());

                        leftLayout->addWidget(widget[0],0,0);
                        leftLayout->addWidget(widget[1],0,1);

                        addMap(1,0,robotvar->getVarType());

                        break;
                }
                case KukaVar::CHAR:
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

                        addChar(1,robotvar->getVarName(),robotvar->getValue());

                        leftLayout->addWidget(widget[0],0,0);
                        leftLayout->addWidget(widget[1],0,1);

                        addMap(1,0,robotvar->getVarType());

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
                case KukaVar::STRUCTURE:
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
                case KukaVar::INT:
		{
			//INT
			robotvar->setValue(((QSpinBox*)widget[1])->text().toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
                case KukaVar::REAL:
		{
			//REAL
			robotvar->setValue(((QDoubleSpinBox*)widget[1])->text().replace(",",".").toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
                case KukaVar::BOOL:
		{
			//BOOL
			robotvar->setValue(((QComboBox*)widget[1])->currentText().toAscii());
			emit writevalue(robotvar->getVarName(),robotvar->getNewValue(), varip);
			break;
		}
                case KukaVar::CHAR:
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
	
        if (robotvar->getVarType()==KukaVar::STRUCTURE)
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
		
                if (robotvar->getVarType()==KukaVar::STRUCTURE)
			((QLabel*)widget[i])->setText(robotvar->getStructureMember(i));
		else
			((QLabel*)widget[i])->setText(robotvar->getVarName());
	}
}

/*!	\brief Inserimento elemento di tipo intero
 *
 *	Inserisce un elemento di tipo intero alla finestra
 *
 */

void RobotVarEdit::addInt(int widgetindex, QByteArray varName, int value){

    //qDebug() << "Indice widget: " << widgetindex << " valore: " << value;

    widget[widgetindex-1] = new QLabel(varName);
    widget[widgetindex] = new QSpinBox();
    ((QSpinBox*)widget[widgetindex])->setRange(-(9999999),(9999999));
    ((QSpinBox*)widget[widgetindex])->setValue(value);
}

/*!	\brief Inserimento elemento di tipo reale
 *
 *	Inserisce un elemento di tipo real alla finestra
 *
 */

void RobotVarEdit::addReal(int widgetIndex, QByteArray varName, double value){
    widget[widgetIndex-1] = new QLabel(varName);
    widget[widgetIndex] = new QDoubleSpinBox();
    ((QDoubleSpinBox*)widget[widgetIndex])->setDecimals(4);
    ((QDoubleSpinBox*)widget[widgetIndex])->setRange(-9999,9999);
    ((QDoubleSpinBox*)widget[widgetIndex])->setValue(value);
}

/*!	\brief Inserimento elemento di tipo char
 *
 *	Inserisce un elemento di tipo char alla finestra
 *
 */

void RobotVarEdit::addChar(int widgetindex, QByteArray varName, QByteArray value){

    widget[widgetindex-1] = new QLabel(varName);
    widget[widgetindex] = new QLineEdit(value);

    //QRegExp regExp("[A-Za-z0-9]{1-30}");
    //((QLineEdit*)widget[widgetindex])->setValidator(new QRegExpValidator(regExp, this));
    connect(widget[widgetindex], SIGNAL(textChanged(const QString &)), this, SLOT(on_namedLineEdit_textChanged(const QString &)));
}

/*!	\brief Inserimento elemento di tipo intero
 *
 *	Inserisce un elemento di tipo intero alla finestra
 *
 */

void RobotVarEdit::addBool(int widgetindex, QByteArray varName, QByteArray value){

    //qDebug() << "Indice widget: " << widgetindex << " valore: " << value;

    widget[widgetindex-1] = new QLabel(varName);
    widget[widgetindex] = new QComboBox();
    ((QComboBox*)widget[widgetindex])->addItem("TRUE");
    ((QComboBox*)widget[widgetindex])->addItem("FALSE");
    ((QComboBox*)widget[widgetindex])->setCurrentIndex(((QComboBox*)widget[widgetindex])->findText(value));

}

/*!	\brief Inserimento elemento di tipo struttura
 *
 *	Inserisce un elemento di tipo struttura alla finestra
 *
 */

void RobotVarEdit::addStructure(int widgetindex, int value){

    //qDebug() << "Indice widget: " << widgetindex << " valore: " << value;
    widget[widgetindex] = new QSpinBox();
    ((QSpinBox*)widget[widgetindex])->setRange(-(9999999),(9999999));
    ((QSpinBox*)widget[widgetindex])->setValue(value);
}

/*!	\brief Inserimento elemento di tipo struttura
 *
 *	Inserisce un elemento di tipo struttura alla finestra
 *
 */

void RobotVarEdit::addMap(int widgetIndex, int mapIndex, int tipodato){
    qDebug() << "Indice widget: " << widgetIndex << " mapIndex: " << mapIndex << " tipo dato: " << tipodato;
    mapper[mapIndex] = new QSignalMapper(this);
    switch(tipodato){
    case KukaVar::BOOL:
        {
            connect(widget[widgetIndex], SIGNAL(currentIndexChanged(const QString&)),mapper[mapIndex], SLOT(map()));
            break;
        }
    case KukaVar::CHAR:
        {
            connect(widget[widgetIndex], SIGNAL(textChanged(const QString&)),mapper[mapIndex], SLOT(map()));
            break;
        }
    default:
        {
            connect(widget[widgetIndex], SIGNAL(valueChanged(const QString&)),mapper[mapIndex], SLOT(map()));
            break;
        }
    }

    mapper[mapIndex]->setMapping(widget[widgetIndex], mapIndex);
    connect(mapper[mapIndex], SIGNAL(mapped(int)), this, SLOT(on_Changed(int)));
}
