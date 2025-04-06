# Stages of Suffering

Mod page on [nexus](https://www.nexusmods.com/skyrimspecialedition/mods/146568)

# ⚙️ Injury Config Guide

This mod supports external JSON configuration files to define how injury spells work.

## 📁 Where to Put JSON Files

Create `.json` files inside the following folder:
```
SKSE/Plugins/SufferingInjuries/
```

Each JSON file represents one injury configuration set. You can have multiple JSON files, and the mod will load all of them.

---

## 🧠 JSON Structure

Here's an example:

```json
{
  "minor_injuries": [
    "0x800|myAwesome.esp",
    "0x801|myAwesome.esp"
  ],
  "medium_injuries": [
    "0x802|myAwesome.esp",
    "0x803|myAwesome.esp"
  ],
  "major_injuries": [
    "0x804|myAwesome.esp",
    "0x805|myAwesome.esp"
  ],
  "spell_downgrades": {
    "0x805|myAwesome.esp": "0x803|myAwesome.esp",
    "0x803|myAwesome.esp": "0x801|myAwesome.esp"
  },
  "spell_upgrades": {
    "0x801|myAwesome.esp": "0x803|myAwesome.esp",
    "0x803|myAwesome.esp": "0x805|myAwesome.esp"
  }
}
```
---

## ✅ Required Fields

- `minor_injuries` — Required
- `medium_injuries` — Required
- `major_injuries` — Required
- `spell_downgrades` — Optional
- `spell_upgrades` — Optional

---

## 🔄 Example Use

If you define upgrades like:
```json
"spell_upgrades": {
  "0x801|myAwesome.esp": "0x803|myAwesome.esp"
}
```
Dying with your injury then upgrades it to the next tier.