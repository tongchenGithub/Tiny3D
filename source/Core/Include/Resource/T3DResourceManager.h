

#ifndef __T3D_RESOURCE_MANAGER_H__
#define __T3D_RESOURCE_MANAGER_H__


#include "T3DPrerequisites.h"
#include "T3DResource.h"


namespace Tiny3D
{
    class T3D_ENGINE_API ResourceManager
    {
    public:
        ResourceManager();
        virtual ~ResourceManager();

        static uint32_t toID(const TString &strName);

        /**
         * @brief Load resource from file.
         */
        virtual ResourcePtr load(const TString &strName);

        /**
         * @brief Unload resource in memory.
         */
        virtual void unload(ResourcePtr &res);

        /**
         * @brief Clone another new resource from source resource.
         * @param [in] pSrcRes
         *      Source resource used to clone.
         * @return retrieve a pointer to new instance of resource.
         * @remarks
         *      All Resource subclass should implement clone() method to
         *      new instance of resource. This method will call 
         *      Resource::clone() to implement this function.
         */
        ResourcePtr clone(const ResourcePtr &rkSrcResource);

        /**
         * @brief Get resource by name.
         * @param [in] strName
         *      The name of resource.
         * @param [in] unCloneID
         *      0 if the resource wasn't created by clone. Default is 0.
         * @return retrieve a pointer to instance of resource.
         */
        ResourcePtr getResource(const TString &strName, 
            uint32_t unCloneID = 0) const;

        /**
         * @brief Get resources by name.
         * @remarks
         *      This method will return all resource in list 
         *      whether the resource created or cloned.
         */
        bool getResources(const TString &strName, 
            std::list<ResourcePtr> &rResList) const;

        void addSearchPath(const TString &strPath);

    protected:
        virtual Resource *create(const TString &strName) = 0;

        static uint32_t hash(const char *str);

    protected:
        typedef std::list<TString> SearchPathList;

        typedef std::map<uint32_t, ResourcePtr>     Resources;
        typedef Resources::iterator                 ResourcesItr;
        typedef Resources::const_iterator           ResourcesConstItr;
        typedef std::pair<uint32_t, ResourcePtr>    ResPairValue;

        typedef std::map<TString, Resources>        ResourcesMap;
        typedef ResourcesMap::iterator              ResourcesMapItr;
        typedef ResourcesMap::const_iterator        ResourcesMapConstItr;

        typedef std::pair<TString, Resources>       ResMapPairValue;

        SearchPathList  m_SearchPathList;   /** search path list */
        ResourcesMap    m_ResourceCache;    /** cache all resources */

        uint32_t        m_unCloneID;        /** used to clone */
    };
}


#endif  /*__T3D_RESOURCE_MANAGER_H__*/