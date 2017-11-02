#include "EditorComponent.h"

void EditorComponent::getEditableProperties (Array<PropertyComponent*>& props) {}

const float EditorComponent::getData(const float position) { return 0.0; }

void EditorComponent::transportEnter() {};

void EditorComponent::transportExit() {};

ValueTree EditorComponent::getForDocument() { return ValueTree ("EDITOR"); };

void EditorComponent::loadFromDocument (ValueTree data) { };