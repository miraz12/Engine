#pragma once 
#include <string>
#include <vector>
#include "property.h"
#include <unordered_map>

namespace Base
{
    class Entity
    {
    public:
        /// an Id type, used to identify entities
        typedef unsigned int EntityId;

        /// constructor
        Entity();
        /// destructor
        virtual ~Entity();

        /// get the entity's category
        const std::string& GetCategory() const;
        /// get unique id of entity
        EntityId GetUniqueId() const;
        /// return true if any property accepts/processes message
        //bool AcceptsMessage(const Messaging::Id& msgId) const;
        /// send a synchronous message to the entity
        //void SendSync(const Ptr<Messaging::Message>& msg);
        /// return true if the entity is currently active (between OnActivate/OnDeactivate)
        bool IsActive() const;
        /// get the instance attribute table for the entity
        //const Ptr<Db::ValueTable>& GetAttrTable() const;
        /// get the instance attribute table row index for the entity
        //IndexT GetAttrTableRowIndex() const;
        /// find property by RTTI (performs IsA() check), returns invalid pointer if not exists, slow!
        //Ptr<Property> FindProperty(const Core::Rtti& rtti) const;
        /// returns true if entity has a property with the given RTTI
        //bool HasProperty(const Core::Rtti& rtti) const;
        /// Returns the entity's property list.
        //const Util::Array<Ptr<Property>>& GetAllProperties() const { return properties; }



        //=== callbacks ===

        /// register a property callback, called by Property::SetupCallback() method
        //void RegisterPropertyCallback(const Ptr<Property>& prop, Property::CallbackType callback);
        /// called when attached to world
        virtual void OnActivate();
        /// called when removed from world
        virtual void OnDeactivate();
        /// called at the beginning of the frame
        void OnBeginFrame();
        /// called before movement
        void OnMoveBefore();
        /// called after movement
        void OnMoveAfter();
        /// called before rendering
        void OnRender();
        /// debug rendering called before rendering 
        void OnRenderDebug();
        /// called if entity loses activity
        void OnLoseActivity();
        /// called if entity gains activity
        void OnGainActivity();
        /// called if entity has deferred messages
        void OnHandleDeferred();
        /// called after loading from database has happened
        void OnLoad();
        /// called when the entity starts to live in the complete world
        void OnStart();
        /// called before saving from database happens
        void OnSave();



    private:
        //friend class Base::EntityManager;


        std::string category;
        //Ptr<Messaging::Dispatcher> dispatcher;
        //Ptr<Db::ValueTable> attrTable;
        //IndexT attrTableRowIndex;
        std::vector<Property> properties;
        std::unordered_map<Property::CallbackType, std::vector<Property*>> callbackProperties;
        EntityId uniqueId;

        static EntityId uniqueIdCounter;

        bool activated;
        bool isInOnActivate;
        bool isInOnDeactivate;
    };
}
