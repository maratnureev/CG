import * as THREE from './libs/three.module.js'
import { OrbitControls } from './src/OrbitControls.mjs'

const CANVAS_WIDTH = 1000
const CANVAS_HEIGHT = 1000

window.onload = () => {
    const canvas = document.getElementById('canvas')

    canvas.setAttribute('width', `${CANVAS_WIDTH}`)
    canvas.setAttribute('height', `${CANVAS_HEIGHT}`)

    const renderer = new THREE.WebGLRenderer({ canvas })
    renderer.setClearColor( 0xffffff, 0);
    const scene = new THREE.Scene()
    const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 5000)

    camera.position.set(0, 0, 5)

    const controls = new OrbitControls(camera, renderer.domElement)
    controls.target.set(0, 0, 0)

    const vertexShader = document.getElementById('vertShader').innerHTML
    const fragmentShader = document.getElementById("fragShader").innerHTML

    const geometry = new THREE.BufferGeometry()

    const endPoint = 2 * Math.PI
    const step = Math.PI / 1000

    const stepIndex = Math.ceil(endPoint / step)
    const vertices = []
    for (let i = 0; i < stepIndex; i++) {
        vertices.push([i * step, 0, 0])
    }
    const verticesFloat = new Float32Array(vertices.flat())

    geometry.setAttribute( 'position', new THREE.BufferAttribute( verticesFloat, 3 ) );
    const material = new THREE.ShaderMaterial({
        vertexShader,
        fragmentShader,
    })
    const line = new THREE.Line(geometry, material)

    scene.add(line)

    function animate () {
        requestAnimationFrame(animate)

        controls.update()
        renderer.render(scene, camera)
    }

    controls.update()
    renderer.render(scene, camera)
    animate()
}