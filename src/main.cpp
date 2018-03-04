#include <OpenImageIO/imageio.h>

#include <iostream>

int main( int argc, char *argv[] )
{
    auto imageInput = OIIO::ImageInput::open(argv[1]);
    if(!imageInput)
        return 1;
    const OIIO::ImageSpec &spec = imageInput->spec();
    struct RGB { uint8_t r; uint8_t g; uint8_t b; };
    std::vector<RGB> colour_pixels(spec.width*spec.height);
    imageInput->read_image(OIIO::TypeDesc::UINT8, colour_pixels.data());
    imageInput->close();

    std::vector<uint8_t> grayscale_pixels;
    grayscale_pixels.reserve(spec.width*spec.height);
    std::transform( colour_pixels.cbegin(), colour_pixels.cend(),
                    std::back_inserter(grayscale_pixels),
                    [] ( const RGB &c )
                    {
                        return 0.21*c.r + 0.72*c.g + 0.07*c.b;
                    });

    auto imageOutput = OIIO::ImageOutput::create(argv[2]);
    if(!imageOutput)
        return 2;
    OIIO::ImageSpec outSpec( spec.width, spec.height, 1, OIIO::TypeDesc::UINT8 );
    if(!imageOutput->open( argv[2], outSpec ))
    {
        std::cerr<<imageOutput->geterror();
        return 3;
    }
    imageOutput->write_image( OIIO::TypeDesc::UINT8, grayscale_pixels.data() );
    imageOutput->close();
    
    OIIO::ImageInput::destroy(imageInput);
    OIIO::ImageOutput::destroy(imageOutput);
    
    std::cout<<"It runs."<<std::endl;
    return 0;
}
