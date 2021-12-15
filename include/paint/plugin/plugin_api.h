#ifndef _PLUGIN_HPP_INCLUDED_
#define _PLUGIN_HPP_INCLUDED_

#include <cstdint>

namespace plugin {

const uint32_t kVersion = 2; //updated version
typedef uint32_t color_t;    //color_t = 0xAA'BB'GG'RR;
    
class ITexture;

struct Buffer {
    color_t* pixels;
    ITexture* texture;
};

struct Rect {
    int32_t x;
    int32_t y;
    int32_t size_x;
    int32_t size_y; 
    int32_t outline_thickness;
    color_t fill_color;
    color_t outline_color;
};

struct Circle {
    int32_t x;
    int32_t y;
    int32_t radius;
    int32_t outline_thickness;
    color_t fill_color;
    color_t outline_color;
};

struct Line {
    int32_t x0;
    int32_t y0;
    int32_t x1;
    int32_t y1;
    int32_t thickness;
    color_t color;
};

class ITexture {
  public:
    virtual ~ITexture() {}

    virtual int32_t GetSizeX() = 0;
    virtual int32_t GetSizeY() = 0;

    virtual Buffer ReadBuffer() = 0;
    virtual void ReleaseBuffer(Buffer buffer) = 0;
    virtual void LoadBuffer(Buffer buffer) = 0;

    virtual void Clear(color_t color) = 0;
    virtual void Present() = 0;

    virtual void DrawLine  (const Line& line) = 0;
    virtual void DrawCircle(const Circle& circle) = 0;
    virtual void DrawRect  (const Rect& rect) = 0;

    virtual void CopyTexture(ITexture* source, int32_t x, int32_t y, int32_t size_x, int32_t size_y) = 0;
    virtual void CopyTexture(ITexture* source, int32_t x, int32_t y) = 0;
};

class ITextureFactory {
  public:
    virtual ~ITextureFactory() {}

    virtual ITexture* CreateTexture(const char* filename) = 0;
    virtual ITexture* CreateTexture(int32_t size_x, int32_t size_y) = 0;
};

class IClickCallback {
  public:
    virtual ~IClickCallback() {}

    virtual void RespondOnClick() = 0;
};

class ISliderCallback {
  public:
    virtual ~ISliderCallback() {}

    virtual void RespondOnSlide(float old_value, float current_value) = 0;
};

class IPaletteCallback {
  public:
    virtual ~IPaletteCallback() {}

    virtual void RespondOnChangeColor(color_t color) = 0;  
};

class IWidget {
  public:
    virtual ~IWidget() {}

    virtual int32_t GetSizeX() = 0;
    virtual int32_t GetSizeY() = 0;
};

class IButton : public IWidget {
  public:
    virtual ~IButton() {}

    virtual void SetClickCallback(IClickCallback* callback) = 0;
};

class ISlider : public IWidget {
  public:
    virtual ~ISlider() {}

    virtual void SetSliderCallback(ISliderCallback* callback) = 0;

    virtual float GetValue() = 0;
    virtual void SetValue(float value) = 0;
};

class ILabel : public IWidget {
  public:
    virtual ~ILabel() {}

    virtual void SetText(const char* text) = 0;
};

class IIcon : public IWidget {
  public:
    virtual ~IIcon() {}

    virtual void SetIcon(const ITexture* icon) = 0;
};

class IPalette : public IWidget {
  public:
    virtual ~IPalette() {}

    virtual void SetPaletteCallback(IPaletteCallback* callback) = 0;
};

class IPreferencesPanel : public IWidget {
  public:
    virtual ~IPreferencesPanel() {}

    virtual void Attach(IButton*  button,  int32_t x, int32_t y) = 0;
    virtual void Attach(ILabel*   label,   int32_t x, int32_t y) = 0;
    virtual void Attach(ISlider*  slider,  int32_t x, int32_t y) = 0;
    virtual void Attach(IIcon*    icon,    int32_t x, int32_t y) = 0;
    virtual void Attach(IPalette* palette, int32_t x, int32_t y) = 0;
};

class IWidgetFactory {
  public:
    virtual ~IWidgetFactory() {}

    virtual IButton* CreateDefaultButtonWithIcon(const char* icon_file_name) = 0;
    virtual IButton* CreateDefaultButtonWithText(const char* text) = 0;
    virtual IButton* CreateButtonWithIcon(int32_t size_x, int32_t size_y, const char* icon_file_name) = 0;
    virtual IButton* CreateButtonWithText(int32_t size_x, int32_t size_y, const char* text, int32_t char_size) = 0;

    virtual ISlider* CreateDefaultSlider(float range_min, float range_max) = 0;
    virtual ISlider* CreateSlider(int32_t size_x, int32_t size_y, float range_min, float range_max) = 0;

    virtual ILabel*  CreateDefaultLabel(const char* text) = 0;
    virtual ILabel*  CreateLabel(int32_t size_x, int32_t size_y, const char* text, int32_t char_size) = 0;

    virtual IIcon*   CreateIcon(int32_t size_x, int32_t size_y) = 0;

    virtual IPalette* CreatePalette() = 0;

    virtual IPreferencesPanel* CreatePreferencesPanel() = 0;
};

class IAPI {
  public:
    virtual ~IAPI() {}

    virtual IWidgetFactory*  GetWidgetFactory () = 0;
    virtual ITextureFactory* GetTextureFactory() = 0;
};

class IFilter {
  public:
    virtual ~IFilter() {}

    virtual void Apply(ITexture* canvas) = 0;
    virtual const char* GetName() const = 0;

    virtual IPreferencesPanel* GetPreferencesPanel() const = 0;
};

class ITool {
  public:
    virtual ~ITool() {}

    virtual void ActionBegin(ITexture* canvas, int x, int y) = 0;
    virtual void Action     (ITexture* canvas, int x, int y, int dx, int dy) = 0;
    virtual void ActionEnd  (ITexture* canvas, int x, int y) = 0;

    virtual const char* GetIconFileName() const = 0;
    virtual const char* GetName() const = 0;

    virtual IPreferencesPanel* GetPreferencesPanel() const = 0;
};

struct Tools {
    ITool** tools;
    uint32_t count;
};

struct Filters {
    IFilter** filters;
    uint32_t count;
};

class IPlugin {
  public:
    virtual ~IPlugin() {}

    virtual Filters GetFilters() const = 0;
    virtual Tools   GetTools()   const = 0;
};

typedef IPlugin* (*CreateFunction) (IAPI* api);
typedef void     (*DestroyFunction)(IPlugin* plugin);
typedef uint32_t (*VersionFunction)();

#ifdef _WIN32 //windows

#define TOOLCALL __cdecl

#ifdef EXPORT_TOOL
  #define TOOLAPI __declspec(dllexport)
#else
  #define TOOLAPI __declspec(dllimport)
#endif

extern "C" TOOLAPI IPlugin* TOOLCALL Create(IAPI* api);
extern "C" TOOLAPI void     TOOLCALL Destroy(IPlugin* plugin);
extern "C" TOOLAPI uint32_t TOOLCALL Version();

#endif

} // namespace plugin

#endif /* _PLUGIN_HPP_INCLUDED_ */