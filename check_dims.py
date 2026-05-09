import struct
import os

path = r'd:\Metal Slug\OOP_Project\Sprites\Clean'
files = sorted([f for f in os.listdir(path) if f.startswith('Eri_') or f.startswith('Marco_')])

for f in files:
    f_path = os.path.join(path, f)
    try:
        with open(f_path, 'rb') as fd:
            data = fd.read(24)
            if len(data) >= 24:
                w, h = struct.unpack('>II', data[16:24])
                print(f'{f}: {w}x{h}')
            else:
                print(f'{f}: Error reading')
    except Exception as e:
        print(f'{f}: {e}')
