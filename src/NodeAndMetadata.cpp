#include "NodeAndMetadata.h"

NodeAndMetadata::NodeAndMetadata(Ogre::SceneNode* node, unsigned int status):
    node(node) {
    for (int i =0 ; i < STATUSLIST ; ++i) {
            statuses[i]=0;
    }

}

NodeAndMetadata::~NodeAndMetadata()
{
    //dtor
}

void NodeAndMetadata::Setcurrentstatus(unsigned int val) {
    if (statuses[0] != val) {
        for (int i =0 ; i < STATUSLIST-1 ; ++i) {
                statuses[i+1]=statuses[i];
        }
        statuses[0] = val;
    }
}

unsigned int NodeAndMetadata::Stepbackstatus() {
    for (int i =1 ; i < STATUSLIST-1 ; ++i) {
                statuses[i-1]=statuses[i];
    }
    statuses[ STATUSLIST ] = 0;
    return statuses[0];
}
