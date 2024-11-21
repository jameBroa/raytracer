import json

def create_checkerboard():
    shapes = []
    size = 0.5
    start_x = -1.0
    start_z = 2.0
    colors = [[1, 1, 1], [0, 0, 0]]

    for i in range(4):
        for j in range(4):
            color = colors[(i + j) % 2]
            x0 = start_x + j * size
            z0 = start_z - i * size
            x1 = x0 + size
            z1 = z0 - size

            # Two triangles per square
            shapes.append({
                "type": "triangle",
                "v0": [x0, -0.5, z0],
                "v1": [x1, -0.5, z0],
                "v2": [x1, -0.5, z1],
                "material": {
                    "ks": 0.1,
                    "kd": 0.9,
                    "specularexponent": 20,
                    "diffusecolor": color,
                    "specularcolor": [1.0, 1.0, 1.0],
                    "isreflective": False,
                    "reflectivity": 1.0,
                    "isrefractive": False,
                    "refractiveindex": 1.0
                }
            })
            shapes.append({
                "type": "triangle",
                "v0": [x0, -0.5, z1],
                "v1": [x0, -0.5, z0],
                "v2": [x1, -0.5, z1],
                "material": {
                    "ks": 0.1,
                    "kd": 0.9,
                    "specularexponent": 20,
                    "diffusecolor": color,
                    "specularcolor": [1.0, 1.0, 1.0],
                    "isreflective": False,
                    "reflectivity": 1.0,
                    "isrefractive": False,
                    "refractiveindex": 1.0
                }
            })

    return shapes

def main():
    data = {
        "nbounces": 8,
        "rendermode": "phong",
        "camera": {
            "type": "pinhole",
            "width": 1200,
            "height": 800,
            "position": [0.0, 1, -2],
            "lookAt": [0.0, -0.1, 1.0],
            "upVector": [0.0, 1.0, 0.0],
            "fov": 45.0,
            "exposure": 0.1
        },
        "scene": {
            "backgroundcolor": [0.25, 0.25, 0.25],
            "lightsources": [
                {
                    "type": "pointlight",
                    "position": [0, 1.0, 0.5],
                    "intensity": [0.75, 0.75, 0.75]
                }
            ],
            "shapes": [
                {
                    "type": "sphere",
                    "center": [0, -0.2, 1],
                    "radius": 0.25,
                    "material": {
                        "ks": 0.1,
                        "kd": 0.9,
                        "specularexponent": 20,
                        "diffusecolor": [0.8, 0.5, 0.5],
                        "specularcolor": [1.0, 1.0, 1.0],
                        "isreflective": False,
                        "reflectivity": 0.0,
                        "isrefractive": True,
                        "refractiveindex": 1.4
                    }
                }
            ] + create_checkerboard()
        }
    }

    with open('refract_balls_checkerboard.json', 'w') as f:
        json.dump(data, f, indent=4)

if __name__ == "__main__":
    main()