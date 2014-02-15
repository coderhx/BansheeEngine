#pragma once

#include "CmPrerequisites.h"
#include "CmModule.h"
#include "CmGameObject.h"

namespace CamelotFramework
{
	class CM_EXPORT GameObjectManager : public Module<GameObjectManager>
	{
	public:
		GameObjectManager();
		~GameObjectManager();

		GameObjectHandleBase registerObject(const std::shared_ptr<GameObject>& object);
		void unregisterObject(const GameObjectHandleBase& object);

		GameObjectHandleBase getObject(UINT64 id) const;
		bool objectExists(UINT64 id) const;

		/************************************************************************/
		/* 							DESERIALIZATION                      		*/
		/************************************************************************/
		// Note: GameObjects need a bit of special handling when it comes to deserialization,
		// which is what this part of the code is used for. It performs two main actions:
		//  - 1. Resolves all GameObjectHandles on deserialization end
		//    - We can't just resolve them as we go because during deserialization not all objects
		//      have necessarily been created.
		//  - 2. Maps serialized IDs to actual in-engine ids. 

		/**
		 * @brief	Needs to be called whenever GameObject deserialization starts. Must be followed
		 * 			by notifyDeserializationEnded call.
		 */
		void notifyDeserializationStarted(GameObject* object);

		/**
		 * @brief	Needs to be called whenever GameObject deserialization ends. Must be preceded
		 * 			by notifyDeserializationStarted call.
		 */
		void notifyDeserializationEnded(GameObject* object);

		/**
		 * @brief	Registers an id that was deserialized, and has been remapped to
		 * 			an actual in-engine ID. This will be used when resolving GameObjectHandles
		 * 			(since they might point to the invalid deserialized id).
		 */
		void registerDeserializedId(UINT64 deserializedId, UINT64 actualId);

		/**
		 * @brief	Queues the specified handle and resolves it when deserialization ends.
		 */
		void registerUnresolvedHandle(const GameObjectHandleBase& object);

	private:
		UINT64 mNextAvailableID; // 0 is not a valid ID
		Map<UINT64, GameObjectHandleBase>::type mObjects;

		GameObject* mActiveDeserializedObject;
		bool mIsDeserializationActive;
		Map<UINT64, UINT64>::type mIdMapping;
		Vector<GameObjectHandleBase>::type mUnresolvedHandles;
	};
}