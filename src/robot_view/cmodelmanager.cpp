#include "cmodelmanager.h"

CModelManager::CModelManager(){

}

void CModelManager::addModel( CGLRenderable* model ){
    int index = m_modelsMap.size() + 10001 ;
    m_modelsMap.insert( index, model );
    model->setSelectIndex( index );
    qDebug()<< "new model: "<< index;
}

CGLRenderable* CModelManager::getModelFromKey( int id ){
    return m_modelsMap.value( id, NULL );
}

void CModelManager::unselectAll(){
    QList<int> keys = m_modelsMap.keys();

    int __i;
    for( __i = 0; __i < keys.size(); __i++ ){
        m_modelsMap.value( keys.at(__i) )->unselect();
    }
}

CGLRenderable* CModelManager::getSelectedObject(){
    QList<int> keys = m_modelsMap.keys();

    int __i;
    for( __i = 0; __i < keys.size(); __i++ ){
        if( ((CGLRenderable*)m_modelsMap.value( keys.at(__i) ))->selected() ){
            return m_modelsMap.value( keys.at(__i) );
        }
    }
    return NULL;
}

bool CModelManager::hasSelectedObject(){
    if( getSelectedObject() != NULL )
        return true;
    return false;
}
