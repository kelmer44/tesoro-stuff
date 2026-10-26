# Renderer

How the Tesoro engine draws a frame. Source: `engines/tesoro/renderer.cpp` and
`renderer.h` in the engine worktree (`scummvm-tesoro`), driven from
`TesoroEngine::draw()` in `tesoro.cpp`.

This describes the current engine implementation. It has not been checked
against the original executable here; fidelity is tracked by the comparison
tools in `tools/` and in `RE_STATUS.md`.

## Frame pipeline

```mermaid
flowchart TD
    A["TesoroEngine::draw()<br/>once per frame, 640x480 ARGB"] --> B["Scene::appendSprites()<br/>each object emits image, x, y, depth, scale"]
    B --> C["1. Clear<br/>black screen, depth buffer = 0x10000"]
    C --> D["2. Unscaled sprites (scale == 0)<br/>per pixel: draw if depth is lower"]
    D --> E["3. Scaled sprites<br/>sorted far to near, same depth test"]
    E --> F["4. Subtitle text<br/>glyphs on top, no depth test"]
    F --> G["Screen::update()"]

    classDef gray fill:#eee,stroke:#666,color:#222
    classDef teal fill:#d9f2ea,stroke:#0f6e56,color:#04342c
    classDef purple fill:#e8e6fb,stroke:#534ab7,color:#26215c
    classDef coral fill:#fae3db,stroke:#993c1d,color:#4a1b0c
    class A,G gray
    class B teal
    class C,D,E purple
    class F coral
```

Outside the adventure state (menus, censorship card) the engine skips this
pipeline and calls `drawImage()` on a black screen: the image is drawn as it
is, with no depth test.

## Data the renderer consumes

```mermaid
classDiagram
    class SceneSprite {
        const Image* image
        int32 x, y
        uint16 depth
        int32 scale
    }
    class Image {
        uint w, h
        kind
        rowStart[]
        spans[]
        pixel(span, k) RGB565
    }
    class ImageSpan {
        skip
        count
    }
    class SceneRenderer {
        -Array~uint32~ _depth
        +draw(surface, sprites)
        +drawNarration(surface, font, text, colour)
        +drawText(surface, font, text, colour, centreX, y, maxWidth, lineHeight)
        -drawScaled(surface, sprite)
    }
    SceneRenderer ..> SceneSprite : reads list
    SceneSprite --> Image
    Image *-- ImageSpan : rows of
```

## Notes

- **Images** store only drawn pixels, as rows of spans (`skip`, `count`).
  Transparent pixels are never stored, so each span is a run of drawn pixels,
  converted from RGB565 to ARGB as it is written.
- **Depth** is a 640x480 buffer. A pixel is written only if
  `sprite.depth < _depth[index]`, so unscaled sprites are order-independent.
- **`scale`** is in 256ths (256 is full size); 0 means unscaled. Scaled sprites
  are drawn after all unscaled ones, farthest first. `drawScaled()` picks source
  pixels in 16.16 fixed point: rows by a vertical step, and each span from where
  it starts and ends once scaled.
- **Text** (`drawText`) wraps to `maxWidth`, centres the block on a point, and
  clamps it to the screen. Characters without a glyph are as wide as a space.
  `drawNarration` is `drawText` centred on the screen; spoken lines are anchored
  above the speaker's head.

## Not implemented

- Shadow images (`kind == 2`) drawn on their own (`drawImage`): `error()`. In the sprite pass they are composed
  since 25 October 2026 (`drawShadow`, unscaled only; see CLICK_WALKING.md).
- Scaling of non-literal images (`kind != 1`): `error()` in `drawScaled`.

Both fail visibly on purpose, so unfinished reachable behaviour is never drawn
wrongly.
