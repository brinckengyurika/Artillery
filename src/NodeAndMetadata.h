#ifndef NODEANDMETADATA_H
#define NODEANDMETADATA_H

#include <string>
#include <OgreSceneNode.h>

#define STATUSLIST 20

class NodeAndMetadata
{
    public:
        NodeAndMetadata(Ogre::SceneNode* node, unsigned int status);
        virtual ~NodeAndMetadata();
        Ogre::SceneNode * Getnode() { return node; }
        void Setnode(Ogre::SceneNode * val) { node = val; }
        unsigned int Getcurrentstatus() { return statuses[0]; }
        void Setcurrentstatus(unsigned int val);
        unsigned int Stepbackstatus();


    protected:

    private:
        Ogre::SceneNode * node; //!< Member variable "node"
        unsigned int statuses[ STATUSLIST ] ; //!< Member variable "prevoiusstatuses"
};

#endif // NODEANDMETADATA_H
