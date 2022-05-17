import * as THREE from '../libs/three.module.js'

const degToRad = Math.PI / 180;

class MouseMoveControls {
  constructor (camera, canvas) {
    this.camera = camera
    this.canvas = canvas

    this.distance = 1
    this.mdown = new THREE.Vector2()
    this.mmove = new THREE.Vector2()
    this.phi = 25
    this.theta = -15
    this.phim = 0
    this.thetam = 0
    this.fov = 53

    this.canvas.onmousedown = this.#handleMouseDown
  }

  setTarget(x, y, z) {
    this.target = new THREE.Vector3(x, y, z)
  }

  #handleMouseDown(e) {
    this.mdown.set(e.clientX, e.clientY);
    this.thetam = this.theta;
    this.phim = this.phi;

    this.canvas.onmousemove = this.#handleMouseMove
    this.canvas.onmouseup = this.#handleMouseUp
  }

  #handleMouseMove(e) {
    this.mmove.set(e.clientX, e.clientY);
    this.theta = -(this.mmove.x - this.mdown.x) * 0.5 + this.thetam;
    this.phi = (this.mmove.y - this.mdown.y) * 0.5 + this.phim;
    this.phi = Math.min(90, Math.max(-90, this.phi));

    this.#updateCamera();
  }

  #handleMouseUp() {
    this.canvas.onmousemove = null
  }

  #updateCamera() {
    this.camera.position.x = this.distance * Math.sin(this.theta * degToRad) * Math.cos(this.phi * degToRad)
    this.camera.position.y = this.distance * Math.sin(this.phi * degToRad)
    this.camera.position.z = this.distance * Math.cos(this.theta * degToRad) * Math.cos(this.phi * degToRad)

    this.camera.lookAt(
      this.target.getComponent(0),
      this.target.getComponent(1),
      this.target.getComponent(2),
    )
  }
}

export {
  MouseMoveControls,
}