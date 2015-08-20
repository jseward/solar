differences from Sins of  Dark Age
---
Root Window event handling was far more complicated. had hooks for before / after children events. Want to keep root window simpler this time and instead require clients to maintain ghost windows in their hierarchy if they require events to be handled before children.
