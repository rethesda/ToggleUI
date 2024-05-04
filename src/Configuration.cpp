#include "Configuration.h"

#include <toml++/toml.hpp>

#include "Util/TOML.h"

void Configuration::Init(bool a_abort)
{
    auto tmp = std::unique_ptr<Configuration>{ new Configuration };
    if (std::filesystem::exists(path)) {
        tmp->Load(a_abort);
    } else {
        // Export default config if config file not exists.
        tmp->Save(a_abort);
    }
    config = std::move(tmp);
}

void Configuration::Load(bool a_abort)
{
    try {
        LoadImpl();
        auto msg = std::format("Successfully loaded configuration from \"{}\".", PathToStr(path));
        SKSE::stl::log_success(msg);
    } catch (const toml::parse_error& e) {
        auto msg = std::format("Failed to load configuration from \"{}\" (error occurred at line {}, column {}): {}.",
            PathToStr(path), e.source().begin.line, e.source().begin.column, e.what());
        SKSE::stl::log_failure(msg, a_abort);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to load configuration from \"{}\": {}.", PathToStr(path), e.what());
        SKSE::stl::log_failure(msg, a_abort);
    }
}

void Configuration::Save(bool a_abort) const
{
    try {
        SaveImpl();
        auto msg = std::format("Successfully saved configuration to \"{}\".", PathToStr(path));
        SKSE::stl::log_success(msg);
    } catch (const std::exception& e) {
        auto msg = std::format("Failed to save configuration to \"{}\": {}.", PathToStr(path), e.what());
        SKSE::stl::log_failure(msg, a_abort);
    }
}

void Configuration::LoadImpl()
{
    auto data = LoadTOMLFile(path);

    LoadTOMLValue(data, "iHotkey"sv, iHotkey);
    LoadTOMLValue(data, "iModifier"sv, iModifier);

    LoadTOMLValue(data, "iHotkeyCompass"sv, iHotkeyCompass);
    LoadTOMLValue(data, "iModifierCompass"sv, iModifierCompass);

    LoadTOMLValue(data, "iHotkeyPlayerBar"sv, iHotkeyPlayerBar);
    LoadTOMLValue(data, "iModifierPlayerBar"sv, iModifierPlayerBar);

    LoadTOMLValue(data, "iHotkeySubtitle"sv, iHotkeySubtitle);
    LoadTOMLValue(data, "iModifierSubtitle"sv, iModifierSubtitle);

    LoadTOMLValue(data, "slHUDNames"sv, slHUDNames);
    LoadTOMLValue(data, "slMenuNames"sv, slMenuNames);
    LoadTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    // Make sure menu names are ordered, so that binary search can be used.
    std::ranges::sort(slMenuNames);
    std::ranges::sort(slBannedMenuNames);
}

void Configuration::SaveImpl() const
{
    toml::table data;

    SaveTOMLValue(data, "iHotkey"sv, iHotkey);
    SaveTOMLValue(data, "iModifier"sv, iModifier);

    SaveTOMLValue(data, "iHotkeyCompass"sv, iHotkeyCompass);
    SaveTOMLValue(data, "iModifierCompass"sv, iModifierCompass);

    SaveTOMLValue(data, "iHotkeyPlayerBar"sv, iHotkeyPlayerBar);
    SaveTOMLValue(data, "iModifierPlayerBar"sv, iModifierPlayerBar);

    SaveTOMLValue(data, "iHotkeySubtitle"sv, iHotkeySubtitle);
    SaveTOMLValue(data, "iModifierSubtitle"sv, iModifierSubtitle);

    SaveTOMLValue(data, "slHUDNames"sv, slHUDNames);
    SaveTOMLValue(data, "slMenuNames"sv, slMenuNames);
    SaveTOMLValue(data, "slBannedMenuNames"sv, slBannedMenuNames);

    SaveTOMLFile(path, data);
}
