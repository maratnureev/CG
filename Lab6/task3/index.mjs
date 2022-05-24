import * as THREE from './libs/three.module.js'
import { OrbitControls } from './src/OrbitControls.mjs'

const CANVAS_WIDTH = 1024
const CANVAS_HEIGHT = 768


function angleToRadian (angle) {
    return angle * Math.PI / 180
}

window.onload = () => {
    const canvas = document.getElementById('canvas')

    canvas.setAttribute('width', `${CANVAS_WIDTH}`)
    canvas.setAttribute('height', `${CANVAS_HEIGHT}`)

    const renderer = new THREE.WebGLRenderer({ canvas })

    const scene = new THREE.Scene()
    const camera = new THREE.PerspectiveCamera(75, CANVAS_WIDTH / CANVAS_HEIGHT, 0.1, 5000)

    camera.position.set(10, 15, 10)

    const controls = new OrbitControls(camera, renderer.domElement)
    controls.target.set(0, 0, 0)

    const vertexShader = document.getElementById('vertShader').innerHTML
    const fragmentShader = document.getElementById('fragShader').innerHTML
    const geometry = new THREE.PlaneGeometry(2 * Math.PI, Math.PI, 25, 25)
    const material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: {
                type: 'f',
                value: Date.now()
            }
        },
        vertexShader,
        fragmentShader,
        side: THREE.DoubleSide,
        wireframe: true,
    })

    const plane = new THREE.Mesh(geometry, material)
    plane.rotateX(angleToRadian(90))

    scene.add(plane)

    const startTime = Date.now();

    const updateUniforms = () => {
        plane.material.uniforms.uTime.value = Date.now() - startTime;
    }

    function animate () {
        requestAnimationFrame(animate)

        controls.update()
        updateUniforms()
        renderer.render(scene, camera)
    }

    controls.update()
    renderer.render(scene, camera)

    animate()
}