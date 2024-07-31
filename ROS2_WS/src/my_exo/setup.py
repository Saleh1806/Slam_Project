from setuptools import find_packages, setup

package_name = 'my_exo'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='hp',
    maintainer_email='hp@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': ["first_node = my_exo.first_node:main",
                            "second_node = my_exo.second_node:main",
                            "third_node = my_exo.third_node:main",
                            "fourth_node = my_exo.fourth_node:main",
                            "selector_node = my_exo.selector_node:main",
                            "input_node = my_exo.input_node:main",
        ],
    },
)
