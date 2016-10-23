#pragma once

#include "../PlayerbotAIAware.h"
#include "Action.h"
#include "Value.h"
#include "NamedObjectContext.h"
#include "Strategy.h"

namespace ai
{
    class AiObjectContext : public PlayerbotAIAware
    {
    public:
        AiObjectContext(PlayerbotAI* ai);
        virtual ~AiObjectContext() = default;

    public:
        virtual Strategy* GetStrategy(std::string name) { return strategyContexts.GetObject(name, ai); }
        virtual set<std::string> GetSiblingStrategy(std::string name) { return strategyContexts.GetSiblings(name); }
        virtual Trigger* GetTrigger(std::string name) { return triggerContexts.GetObject(name, ai); }
        virtual Action* GetAction(std::string name) { return actionContexts.GetObject(name, ai); }
        virtual UntypedValue* GetUntypedValue(std::string name) { return valueContexts.GetObject(name, ai); }

        template<class T>
        Value<T>* GetValue(std::string name)
        {
            return dynamic_cast<Value<T>*>(GetUntypedValue(name));
        }

        template<class T>
        Value<T>* GetValue(std::string name, std::string param)
        {
            return GetValue<T>((string(name) + "::" + param));
        }

        template<class T>
        Value<T>* GetValue(std::string name, uint32 param)
        {
            std::ostringstream out; out << param;
            return GetValue<T>(name, out.str());
        }

        set<std::string> GetSupportedStrategies()
        {
            return strategyContexts.supports();
        }

        std::string FormatValues()
        {
            std::ostringstream out;
            set<std::string> names = valueContexts.GetCreated();
            for (const auto & name : names)
            {
                UntypedValue* value = GetUntypedValue(name);
                if (!value)
                    continue;

                std::string text = value->Format();
                if (text == "?")
                    continue;

                out << "{" << name << "=" << text << "} ";
            }
            return out.str();
        }

    public:
        virtual void Update();
        virtual void Reset();
        virtual void AddShared(NamedObjectContext<UntypedValue>* sharedValues)
        {
            valueContexts.Add(sharedValues);
        }

    protected:
        NamedObjectContextList<Strategy> strategyContexts;
        NamedObjectContextList<Action> actionContexts;
        NamedObjectContextList<Trigger> triggerContexts;
        NamedObjectContextList<UntypedValue> valueContexts;
    };
}
