#include <OGRE/Ogre.h>

class MaterialListener : 
    public Ogre::MaterialManager::Listener
{
    protected:
        Ogre::MaterialPtr mBlackMat;
    public:
        MaterialListener()
        {
            mBlackMat = Ogre::MaterialManager::getSingleton().create("mBlack", "Internal");
            mBlackMat->getTechnique(0)->getPass(0)->setDiffuse(0,0,0,0);
            mBlackMat->getTechnique(0)->getPass(0)->setSpecular(0,0,0,0);
            mBlackMat->getTechnique(0)->getPass(0)->setAmbient(0,0,0);
            mBlackMat->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,0);
        }

        Ogre::Technique *handleSchemeNotFound(unsigned short, const Ogre::String& schemeName, 
                Ogre::Material*, unsigned short, const Ogre::Renderable*)
        {
            if (schemeName == "Scheme/Glow")
                return mBlackMat->getTechnique(0);
        }
};