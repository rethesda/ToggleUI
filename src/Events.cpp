#include "Events.h"

#include "Application.h"
#include "HotkeyManager.h"

void InputEventSink::Register()
{
    if (auto manager = RE::BSInputDeviceManager::GetSingleton()) {
        manager->AddEventSink(GetSingleton());
        SKSE::log::info("Successfully registered input event.");
    } else {
        SKSE::log::error("Failed to register input event.");
    }
}

RE::BSEventNotifyControl InputEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event || !RE::Main::GetSingleton()->gameActive) {
        return RE::BSEventNotifyControl::kContinue;
    }

    HotkeyManager::Process(a_event);

    return RE::BSEventNotifyControl::kContinue;
}

void MenuOpenCloseEventSink::Register()
{
    if (auto ui = RE::UI::GetSingleton()) {
        ui->AddEventSink(GetSingleton());
        SKSE::log::info("Successfully registered menu open/close event.");
    } else {
        SKSE::log::error("Failed to register menu open/close event.");
    }
}

RE::BSEventNotifyControl MenuOpenCloseEventSink::ProcessEvent(const Event* a_event, [[maybe_unused]] EventSource*)
{
    if (!a_event) {
        return RE::BSEventNotifyControl::kContinue;
    }

#ifdef _DEBUG
    auto ui = RE::UI::GetSingleton();

    std::vector<std::string_view> menuNames;
    std::vector<std::string_view> menuNamesOnStack;
    for (auto& [name, entry] : ui->menuMap) {
        menuNames.push_back(name);
        if (entry.menu && entry.menu->OnStack()) {
            menuNamesOnStack.push_back(name);
        }
    }

    auto controlMap = RE::ControlMap::GetSingleton();

    std::vector<std::uint32_t> contextPriorityStack;
    for (auto inputContext : controlMap->contextPriorityStack) {
        contextPriorityStack.push_back(inputContext);
    }

    SKSE::log::trace(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    SKSE::log::trace("{} {}.", a_event->opening ? "Open" : "Close", std::string_view{ a_event->menuName });
    SKSE::log::trace("Menus: {}.", menuNames);
    SKSE::log::trace("Menus On Stack: {}.", menuNamesOnStack);
    SKSE::log::trace("Context Priority Stack: {}.", contextPriorityStack);
    SKSE::log::trace("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
#endif

    if (!a_event->opening) {
        auto app = Application::GetSingleton();
        if (app->IsMenu(a_event->menuName)) {
            app->ResetUI();
        }
    }
    return RE::BSEventNotifyControl::kContinue;
}
