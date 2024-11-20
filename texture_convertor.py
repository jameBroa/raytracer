from PIL import Image

def convert_png_to_ascii_ppm(input_path, output_path):
    # Open the image
    img = Image.open(input_path)
    
    # Convert image to RGB mode if it's not already
    img = img.convert('RGB')
    
    # Get image dimensions
    width, height = img.size
    
    # Write PPM file (ASCII P3 format)
    with open(output_path, 'w') as f:
        # PPM header
        f.write(f"P3\n{width} {height}\n255\n")
        
        # Write pixel data
        for y in range(height):
            for x in range(width):
                r, g, b = img.getpixel((x, y))
                f.write(f"{r} {g} {b}\n")
            # f.write("\n")
    
    print(f"Converted {input_path} to {output_path}")

# Example usage
convert_png_to_ascii_ppm('brick-wall.jpg', 'brick.ppm')