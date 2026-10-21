# Gesture-Controlled Planetarium

A Unity/C# planetarium interaction prototype using Ultraleap hand tracking.

## Objective and approach

Explore a planetarium through hand gestures and scene-based visual interaction. The [scripts](Assets/Scripts/) include swipe and pose detectors, hand-close and exit interactions, planetary rotation, zoom-related artifact visibility and map navigation. [ArtifactManager.cs](Assets/Scripts/ArtifactManager.cs) subscribes to zoom completion and enables the artifact associated with the selected planet.

## Implementation and results

The archive includes scenes, scripts, assets and package/project configuration. It does not include user-study results or quantitative usability measurements. The former README was a Spanish design notebook containing proposals, accessibility ideas and unfinished tasks; its full text is preserved in [original design notes](design-notes.es.md). Those ideas must not be read as completed features. Existing imported model credits remain alongside their assets.

## Running the project

Open this directory as a Unity project using **6000.2.6f2**, recorded in [ProjectVersion.txt](ProjectSettings/ProjectVersion.txt). Restore the packages declared in [manifest.json](Packages/manifest.json), including **com.ultraleap.tracking 7.2.0** and Universal Render Pipeline **17.2.0**. Hand-controlled interactions require compatible tracking hardware/runtime. Open the archived scenes and inspect their configured objects; no hardware session or editor build was validated during documentation.

## Academic context

Human-Computer Interaction, Intelligent Computing and Systems track, Double Degree in Mathematics and Computer Science, University of Granada. The design notes use collaborative language and refer to shared museum design work; this is presented as collaborative coursework rather than sole authorship. Individual contribution boundaries are not established by the archive. Original model attributions and design notes are retained.
