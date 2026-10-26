# Tesoro engine classes

The `Tesoro` namespace in `engines/tesoro` (branch `tesoro-engine`): about a dozen classes in eleven headers. Four views, from the whole engine down to the data it loads.

## Overview: who owns what

`TesoroEngine` is both the ScummVM `Engine` and the `ScriptHost` that scripts call back into. It owns the archive, the loaded scenes, Jim, the script library and the running scripts.

```mermaid
classDiagram
direction LR
class Engine
class ScriptHost {
  <<interface>>
  gameTime()
  scene(scope, level)
  startScript()
  startSpeech()
  startMovie()
  doNative()
  walk()
  look()
}
class TesoroEngine {
  -State _state
  -uint32 _gameTime
  -uint8 _variables[256]
  -loadScene(name)
  -placeJim()
  -updateScripts()
  -updateSpeech()
  +run()
}
class Archive {
  +open()
  +read(name)
  +stream(name)
}
class SceneRenderer {
  +draw(surface, sprites)
  +drawText()
  +drawNarration()
}
class Scene {
  +camera()
  +find(name)
  +send(msg)
  +appendSprites()
  +addObject()
}
class JimObject
class ScriptLibrary {
  +load(archive)
  +find(id)
  +findText(id)
}
class Script {
  +start()
  +update()
  +actionDone()
}
class Menu
Engine <|-- TesoroEngine
ScriptHost <|.. TesoroEngine
TesoroEngine *-- Archive
TesoroEngine *-- SceneRenderer
TesoroEngine *-- ScriptLibrary
TesoroEngine *-- "2" Menu : main, new game
TesoroEngine o-- "0..*" Scene : _scenes (by file name)
TesoroEngine o-- Scene : _currentScene
TesoroEngine o-- JimObject : _jim
TesoroEngine o-- "0..*" Script : _scripts
Script --> ScriptHost : calls back
Script --> Scene : looks up objects
ScriptLibrary ..> Archive : reads
Scene o-- JimObject : added at runtime
```

## Scene objects

Everything in a room derives from `SceneObject`, which holds one `AnimationPlayer` per animation bank and handles talking. The `kind` numbers come from the original scene files. Kinds without a class yet are built as inert placeholders with a warning.

```mermaid
classDiagram
direction TB
class SceneObject {
  <<abstract>>
  -uint32 _flags
  -uint32 _kind
  -uint64 _nameId
  -int32 _x, _y
  -uint16 _depth
  -int32 _parallax
  -TalkConfig _talk
  +play(bank, now)
  +prepareFrame(now)*
  +sprite(view, out)*
  +backSprite(view, out)*
  +beginSpeech() endSpeech()
  +turn(direction)*
  +speechAnchor()*
}
class PropObject {
  kind 3: backgrounds, ambient
}
class InteractiveObject {
  kinds 6, 9, 12, 13, 17, 20
}
class CharacterObject {
  kind 5
  -uint _idleBank
  +turn()
}
class FloorObject {
  kind 7: walkable ground
  +scaleAt(x, y)
}
class JimObject {
  not in any scene file
  -bool _walking
  -Script* _waiter
  -int32 _scale
  +place(scene, x, y)
  +startWalk(x, y, waiter)
  +update(now)
  +commitPosition()
  +look(direction)
}
class AnimationPlayer {
  +play(now, first, last, loop)
  +update(now)
  +pause() resume()
  +currentFrame()
}
class AnimationState {
  flags, frame
  firstFrame, lastFrame
  cycleDuration
}
class Animation {
  name
  frames
  images
}
class Scene {
  -Group[] _groups
  -SceneObject[] _objects
  -SceneCamera _camera
  -RoomEntry _entry
}
class SceneCamera {
  +setPosition(x, y)
  +groupView(parallax)
}
class SceneSprite {
  image, x, y, depth, scale
}
SceneObject <|-- PropObject
SceneObject <|-- InteractiveObject
SceneObject <|-- FloorObject
SceneObject <|-- JimObject
InteractiveObject <|-- CharacterObject
SceneObject *-- "1..*" AnimationPlayer : one per bank
AnimationPlayer *-- AnimationState
AnimationPlayer --> Animation : plays
SceneObject o-- "1..*" Animation : _banks
Scene o-- "0..*" SceneObject : by parallax group
Scene *-- SceneCamera
Scene --> FloorObject : _floor
SceneObject ..> SceneSprite : produces each frame
```

## Scripts and the host

A `Script` runs one `ScriptBlock` until an op has to wait (sleep, speech, a walk, a menu), then `update` resumes it. Anything a script needs from the game goes through `ScriptHost`. `doNative` receives the block's `nativeType` and the DO ordinal, which is where native object methods live.

```mermaid
classDiagram
direction LR
class ScriptLibrary {
  -TextTable[] _tables
  -ScriptBlock[] _blocks
  +addScripts(file, data)
  +addTextTable(data)
  +link()
  +find(id)
  +findText(id)
}
class ScriptBlock {
  name, file, id
  nativeType
  participants
  ops
  labels
  +findLabel(id)
}
class ScriptOp
class ScriptLabel {
  id
  index
}
class TextTable {
  entries
}
class TextEntry
class Script {
  -uint _pc
  -bool _finished
  -SceneObject* _speaker
  -SceneObject*[] _participants
  -TextEntry*[] _choices
  +start()
  +update()
  +actionDone()
  +setScene(scene)
}
class ScriptHost {
  <<interface>>
  walk(object, x, y, waiter)
  look(object, direction)
  startSpeech(speaker, text)
  pickDialogueChoice()
  startMovie() playSound() playMusic()
  doNative(type, ordinal, script)
  startScript(block, label, vars, scene)
}
class TesoroEngine
class Scene
class SceneObject
class JimObject
ScriptLibrary *-- "0..*" ScriptBlock
ScriptLibrary *-- "0..*" TextTable
TextTable *-- TextEntry
ScriptBlock *-- "0..*" ScriptOp
ScriptBlock *-- "0..*" ScriptLabel
Script --> ScriptBlock : runs
Script --> ScriptHost : _host
Script --> Scene : _scene
Script --> "0..*" SceneObject : participants
ScriptHost <|.. TesoroEngine
TesoroEngine o-- "0..*" Script : _scripts
TesoroEngine ..> Scene : send messages
JimObject --> Script : _waiter, resumed on arrival
Script ..> TextEntry : speech, menus
```

## Resources: what the archive decodes into

Plain structs from `resources.h`. Images stay as the file stores them (rows of spans) and are expanded only when drawn. `SceneData` is the parsed room file that a `Scene` is built from.

```mermaid
classDiagram
direction LR
class Archive {
  -File _dat
  -HashMap _entries
  +open()
  +read(name)
  +has(name)
}
class Entry {
  offset
  size
}
class SceneData
class SceneGroup {
  depthBase, depthSpan
  parallax
}
class SceneObjectRecord {
  name, x, y, depth
  kind
  parameters
  bankIndices
}
class Animation {
  isFont
  name
}
class Frame {
  x, y, image
  hit rectangle
  duration, type
}
class FloorPoint {
  x, y, scale
}
class PathPoint {
  x, y
}
class Image {
  w, h, kind
  literals
  +pixel(span, k)
}
class ImageSpan {
  skip, count, value
}
class Menu
class MenuButton {
  name, bounds, action
}
class Scene
Archive *-- "0..*" Entry
Archive ..> SceneData : readScene()
Archive ..> Animation : readAnimation()
Archive ..> Menu : readMenu()
SceneData *-- "0..*" SceneGroup
SceneData o-- "0..*" Animation : banks
SceneGroup *-- "0..*" SceneObjectRecord
Animation *-- "0..*" Frame
Animation o-- "0..*" Image
Frame *-- "0..*" FloorPoint : type 4
Frame *-- "0..*" PathPoint : type 2
Image *-- "0..*" ImageSpan
Menu *-- "0..*" MenuButton
Menu o-- Image : background
Scene ..> SceneData : built from
```
