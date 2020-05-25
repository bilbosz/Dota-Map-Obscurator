#ifndef OBS_STUDIO_GRAPHICS_SCOPE_HPP
#define OBS_STUDIO_GRAPHICS_SCOPE_HPP

class GraphicsScope
{
public:
    GraphicsScope( bool fake = false );
    ~GraphicsScope();
private:
    bool m_fake;
    static int m_count;
};


#endif //OBS_STUDIO_GRAPHICS_SCOPE_HPP
