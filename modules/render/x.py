import numpy as np
from PIL import Image
from scipy.ndimage import gaussian_filter

def create_realistic_pencil_noise(size=512, num_layers=5):
    # 最终结果
    result = np.zeros((size, size))
    
    # 创建多层噪声
    for i in range(num_layers):
        # 基础噪声
        noise = np.random.rand(size, size)
        
        # 高斯模糊，每层使用不同的sigma
        sigma = 1 + i * 0.5
        noise = gaussian_filter(noise, sigma=sigma)
        
        # 添加方向性条纹
        angle = np.random.rand() * np.pi
        x = np.linspace(0, 20, size)
        y = np.linspace(0, 20, size)
        X, Y = np.meshgrid(x, y)
        stripes = np.sin(X * np.cos(angle) + Y * np.sin(angle)) * 0.5 + 0.5
        
        # 混合并添加到结果中
        weight = 1.0 / (i + 1)
        result += (noise * 0.7 + stripes * 0.3) * weight
    
    # 标准化
    result = result / result.max()
    
    # 调整对比度
    result = np.power(result, 1.2)
    
    # 转换到0-255范围
    result = (result * 255).astype(np.uint8)
    
    # 创建图像并保存
    img = Image.fromarray(result)
    img.save('pencil_noise.png')

create_realistic_pencil_noise()