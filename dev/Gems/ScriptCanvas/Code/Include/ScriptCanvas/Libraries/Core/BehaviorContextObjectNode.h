/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <ScriptCanvas/Core/Node.h>
#include <ScriptCanvas/Core/PureData.h>
#include <ScriptCanvas/Core/Graph.h>
#include <ScriptCanvas/Core/Datum.h>

namespace AZ
{
    class BehaviorClass;
    struct BehaviorValueParameter;
    class ReflectContext;
}

namespace ScriptCanvas
{
    namespace Nodes
    {
        namespace Core
        {
            class BehaviorContextObjectNode
                : public PureData
            {
            public:
                //\todo move to behavior context
                enum ParameterCount
                {
                    Getter = 1,
                    Setter = 2,
                };

                AZ_COMPONENT(BehaviorContextObjectNode, "{4344869D-2543-4FA3-BCD0-B6DB1E815928}", PureData);

                static void Reflect(AZ::ReflectContext* reflectContext);

                BehaviorContextObjectNode() = default;

                ~BehaviorContextObjectNode() = default;

                AZStd::string GetDebugName() const override;            
                
                void InitializeObject(const AZ::Uuid& azType);

                void InitializeObject(const AZStd::string& classNameString);
                
                void InitializeObject(const Data::Type& type);

                void Visit(NodeVisitor& visitor) const override { visitor.Visit(*this); }

                void OnWriteEnd();

            protected:
                void InitializeObject(const AZ::BehaviorClass& behaviorClass);
                void InitializeProperties(const AZ::BehaviorClass& behaviorClass);
                void OnInit() override;
                void OnActivate() override;
                
                void SetInput(const Datum& input, const SlotId& id) override;

                void SetProperty(const Datum& input, const SlotId& id);

                void ConfigureProperties(const AZ::BehaviorClass& behaviorClass);
                bool IsConfigured() { return m_configured; }
                
            private:                
                AZStd::recursive_mutex m_mutex; // post-serialization
                AZStd::string m_className;

                template<typename t_GetterFunction, typename t_SetterFunction>
                struct PropertyAccount
                {
                    AZStd::vector<t_GetterFunction> m_getters;
                    AZStd::unordered_map<SlotId, AZ::u32> m_getterIndexByInputSlot;
                    AZStd::vector<AZ::u32> m_getterSlotIndices;
                    AZStd::unordered_map<SlotId, t_SetterFunction> m_settersByInputSlot;
                };
                PropertyAccount<AZ::BehaviorMethod*, AZ::BehaviorMethod*> m_propertyAccount;
                bool m_configured = false;

                BehaviorContextObjectNode(const BehaviorContextObjectNode&) = delete;
            };
        }
    }
}