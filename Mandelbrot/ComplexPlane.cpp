#include "ComplexPlane.h"
using namespace std;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixel_size = Vector2f(pixelWidth, pixelHeight);
    m_aspectRatio = pixelHeight / static_cast<float>(pixelWidth);
    m_plane_center = Vector2f(0, 0);
    m_plane_size = Vector2f(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio);
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray = VertexArray();
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRenderer()
{
    float pixelHeight = m_pixel_size.y;
    float pixelWidth = m_pixel_size.x;

    if (m_state == CALCULATING)
    {
        for (size_t i = 0; i < pixelHeight; i++)
        {
            for (size_t j = 0; j < pixelWidth; j++)
            {
                m_vArray[j + i * pixelWidth].position = { (float)j, (float)i };
                Vector2f pToC = ComplexPlane::mapPixelToCoords({ (int)m_vArray[j + i * pixelWidth].position.x, (int)m_vArray[j + i * pixelWidth].position.y });
                int it = ComplexPlane::countIterations(pToC);
                Uint8 r;
                Uint8 g;
                Uint8 b;
                ComplexPlane::iterationsToRGB(it, r, g, b);
                m_vArray[j + i * pixelWidth].color = { r, g, b };
            }
        }
        m_state = DISPLAYING;
    }
}
void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_plane_size = Vector2f(xSize, ySize);
    m_state = State::DISPLAYING;
}
void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    float xSize = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
    m_plane_size = Vector2f(xSize, ySize);
    m_state = State::DISPLAYING;
}
void ComplexPlane::setCenter(Vector2i mousePixel)
{
    Vector2f coord = mapPixelToCoords(mousePixel);
    m_plane_center = coord;
    m_state = CALCULATING;
}
void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
    m_mouseLocation = ComplexPlane::mapPixelToCoords(mousePixel);
}
void ComplexPlane::loadText(Text& text)
{
    stringstream stream;
    stream << "Mandelbrot Set\n"
        << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")\n"
        << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")\n"
        << "Left-click to Zoom in\nRight-click to Zoom out\n";
    text.setString(stream.str());
}
int ComplexPlane::countIterations(Vector2f coord)
{
    complex<double> c = { coord.x, coord.y };
    complex<double> z = c;
    int i = 0;
    while((z.real() + z.real() + z.imag() + z.imag()) < 4.0 && i < 64)
    {
        z = z * z + c;
        i++;
    }
    return i;
}
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count >= MAX_ITER)
    {
        r = 0;
        g = 0;
        b = 0;
        return;
    }

    if (count >= 40)
    {
        r = 255;
        g = 0;
        b = 0;
    }
    else if (count >= 30)
    {
        r = 255;
        g = 100;
        b = 0;
    }
    else if (count >= 20)
    {
        r = 0;
        g = 255;
        b = 0;
    }
    else if (count >= 10)
    {
        r = 0;
        g = 100;
        b = 255;
    }
    else
    {
        r = 100;
        g = 0;
        b = 255;
    }
}
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    float xCoord = ((float)mousePixel.x / m_pixel_size.x) * m_plane_size.x
        + (m_plane_center.x - m_plane_size.x / 2.0f);

    float yCoord = ((float)mousePixel.y / m_pixel_size.y) * m_plane_size.y
        + (m_plane_center.y - m_plane_size.y / 2.0f);

    Vector2f coord = Vector2f(xCoord, yCoord);
    return coord;
}