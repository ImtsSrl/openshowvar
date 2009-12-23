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


#include "showvarconst.h"

#include <QApplication>
//#include <kaboutdata.h>

#include "openshowvartree.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setStyle("Plastique");

	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(QString("openshowvar_") + locale);
	app.installTranslator(&translator);
	
	//app.setQuitOnLastWindowClosed(false);
	
	OpenShowVarTree *showvar = new OpenShowVarTree;
	
	Qt::WindowFlags flags = showvar->windowFlags();
	//showvar->setWindowFlags(flags  | Qt::WindowStaysOnTopHint | Qt::CustomizeWindowHint);

	showvar->setAttribute(Qt::WA_DeleteOnClose);
	showvar->move(100,100);
	showvar->show();
        showvar->setWindowTitle("OpenShowVar Beta 0.6.3");
	
	return app.exec();
}

