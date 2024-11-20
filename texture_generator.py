import random
import math

def generate_sphere_texture(width=256, height=256):
    # Create PPM file with procedural sphere-like texture
    with open('sphere_texture.ppm', 'w') as f:
        # PPM header
        f.write(f"P3\n{width} {height}\n255\n")
        
        # Center of the texture
        cx, cy = width // 2, height // 2
        
        for y in range(height):
            for x in range(width):
                # Calculate distance from center
                dx = x - cx
                dy = y - cy
                distance = math.sqrt(dx*dx + dy*dy)
                max_distance = math.sqrt((cx*cx) + (cy*cy))
                
                # Normalize distance
                norm_dist = distance / max_distance
                
                # Create color gradient with some randomness
                r = int(200 * (1 - norm_dist) + random.randint(-20, 20))
                g = int(150 * (1 - norm_dist) + random.randint(-20, 20))
                b = int(255 * (1 - norm_dist) + random.randint(-20, 20))
                
                # Ensure color values are in valid range
                r = max(0, min(255, r))
                g = max(0, min(255, g))
                b = max(0, min(255, b))
                
                f.write(f"{r} {g} {b} ")
            f.write("\n")
    
    print("Texture map generated: sphere_texture.ppm")

# Generate the texture
generate_sphere_texture()