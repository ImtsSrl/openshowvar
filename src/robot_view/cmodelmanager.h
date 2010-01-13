#ifndef CMODELMANAGER_H
#define CMODELMANAGER_H

#include <QtCore>
#include "cglrenderable.h"

class CModelManager{
public:
    CModelManager();

    void addModel( CGLRenderable* model );

    CGLRenderable* getModelFromKey( int id );

    void unselectAll();

    CGLRenderable* getSelectedObject();

    bool hasSelectedObject();

private:
    QMap<int,CGLRenderable*> m_modelsMap;

};

#endif // CMODELMANAGER_H
