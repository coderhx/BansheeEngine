//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsScriptInputConfiguration.h"
#include "BsMonoManager.h"
#include "BsMonoClass.h"
#include "BsMonoMethod.h"
#include "BsMonoUtil.h"
#include "BsVirtualInput.h"

namespace bs
{
	Map<UINT64, ScriptInputConfiguration*> ScriptInputConfiguration::ScriptInputConfigurations;

	ScriptInputConfiguration::ScriptInputConfiguration(MonoObject* instance, const SPtr<InputConfiguration>& inputConfig)
		:ScriptObject(instance), mInputConfig(inputConfig)
	{ 
		UINT64 configId = (UINT64)inputConfig.get();
		ScriptInputConfigurations[configId] = this;
	}

	void ScriptInputConfiguration::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_CreateInstance", &ScriptInputConfiguration::internal_CreateInstance);
		metaData.scriptClass->addInternalCall("Internal_RegisterButton", &ScriptInputConfiguration::internal_RegisterButton);
		metaData.scriptClass->addInternalCall("Internal_UnregisterButton", &ScriptInputConfiguration::internal_UnregisterButton);
		metaData.scriptClass->addInternalCall("Internal_RegisterAxis", &ScriptInputConfiguration::internal_RegisterAxis);
		metaData.scriptClass->addInternalCall("Internal_UnregisterAxis", &ScriptInputConfiguration::internal_UnregisterAxis);
		metaData.scriptClass->addInternalCall("Internal_SetRepeatInterval", &ScriptInputConfiguration::internal_SetRepeatInterval);
		metaData.scriptClass->addInternalCall("Internal_GetRepeatInterval", &ScriptInputConfiguration::internal_GetRepeatInterval);
	}

	ScriptInputConfiguration* ScriptInputConfiguration::getScriptInputConfig(const SPtr<InputConfiguration>& inputConfig)
	{
		UINT64 configId = (UINT64)inputConfig.get();

		auto iterFind = ScriptInputConfigurations.find(configId);
		if (iterFind != ScriptInputConfigurations.end())
			return iterFind->second;

		return nullptr;
	}

	ScriptInputConfiguration* ScriptInputConfiguration::createScriptInputConfig(const SPtr<InputConfiguration>& inputConfig)
	{
		MonoObject* instance = metaData.scriptClass->createInstance(false);

		ScriptInputConfiguration* nativeInstance = new (bs_alloc<ScriptInputConfiguration>()) ScriptInputConfiguration(instance, inputConfig);
		return nativeInstance;
	}

	void ScriptInputConfiguration::internal_CreateInstance(MonoObject* object)
	{
		SPtr<InputConfiguration> inputConfig = VirtualInput::createConfiguration();

		new (bs_alloc<ScriptInputConfiguration>()) ScriptInputConfiguration(object, inputConfig);
	}

	void ScriptInputConfiguration::internal_RegisterButton(ScriptInputConfiguration* thisPtr, MonoString* name, ButtonCode buttonCode,
		ButtonModifier modifiers, bool repeatable)
	{
		String nameStr = MonoUtil::monoToString(name);

		thisPtr->getInternalValue()->registerButton(nameStr, buttonCode, modifiers, repeatable);
	}

	void ScriptInputConfiguration::internal_UnregisterButton(ScriptInputConfiguration* thisPtr, MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);

		thisPtr->getInternalValue()->unregisterButton(nameStr);
	}

	void ScriptInputConfiguration::internal_RegisterAxis(ScriptInputConfiguration* thisPtr, MonoString* name, InputAxis type, float deadZone,
		float sensitivity, bool invert)
	{
		String nameStr = MonoUtil::monoToString(name);

		VIRTUAL_AXIS_DESC axisDesc;
		axisDesc.type = (UINT32)type;
		axisDesc.deadZone = deadZone;
		axisDesc.invert = invert;
		axisDesc.sensitivity = sensitivity;

		thisPtr->getInternalValue()->registerAxis(nameStr, axisDesc);
	}

	void ScriptInputConfiguration::internal_UnregisterAxis(ScriptInputConfiguration* thisPtr, MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);

		thisPtr->getInternalValue()->unregisterAxis(nameStr);
	}

	void ScriptInputConfiguration::internal_SetRepeatInterval(ScriptInputConfiguration* thisPtr, UINT64 milliseconds)
	{
		thisPtr->getInternalValue()->setRepeatInterval(milliseconds);
	}

	UINT64 ScriptInputConfiguration::internal_GetRepeatInterval(ScriptInputConfiguration* thisPtr)
	{
		return thisPtr->getInternalValue()->getRepeatInterval();
	}

	void ScriptInputConfiguration::_onManagedInstanceDeleted()
	{
		UINT64 configId = (UINT64)mInputConfig.get();
		ScriptInputConfigurations.erase(configId);

		ScriptObject::_onManagedInstanceDeleted();
	}

	ScriptVirtualAxis::ScriptVirtualAxis(MonoObject* instance)
		:ScriptObject(instance)
	{ }

	void ScriptVirtualAxis::initRuntimeData()
	{
		metaData.scriptClass->addInternalCall("Internal_InitVirtualAxis", &ScriptVirtualAxis::internal_InitVirtualAxis);
	}

	UINT32 ScriptVirtualAxis::internal_InitVirtualAxis(MonoString* name)
	{
		String nameStr = MonoUtil::monoToString(name);

		VirtualAxis vb(nameStr);
		return vb.axisIdentifier;
	}
}