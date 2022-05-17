import * as THREE from '../libs/three.module.js'
import { AmbientLight } from '../libs/three.module.js'

const MILLISECONDS_PER_SECOND = 1000
const SECONDS_PER_MINUTE = 60
const MILLISECONDS_PER_MINUTE = MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE

const EARTH_OPTIONS = {
  aroundAxis: MILLISECONDS_PER_MINUTE,
  aroundSun: 10 * MILLISECONDS_PER_MINUTE,
  orbit: 4000,
  radius: 150,
}

const MOON_OPTIONS = {
  aroundAxis: MILLISECONDS_PER_MINUTE,
  aroundEarth: MILLISECONDS_PER_MINUTE,
  orbit: 700,
  radius: 40,
}

const SUN_OPTIONS = {
  aroundAxis: MILLISECONDS_PER_MINUTE * 5,
  radius: 400,
}

const EARTH_ROTATE_Z_ANGLE = 23.43

function angleToRadian (angle) {
  return angle * Math.PI / 180
}

class SolarSystemScene {

  constructor () {
    this.scene = new THREE.Scene()

    this.scene.add(new AmbientLight(0xffffff, 0.5));

    this.earthCurrentYAngle = 0
    this.sunCurrentYAngle = 0
    this.moonCurrentYAngle = 0

    this.startTimer = Date.now()

    this.#createBackground()
    this.#createSun()
    this.#createEarth()
    this.#createMoon()
  }

  updateSceneAfterFrame () {
    const currentTime = Date.now()
    const timeSpent = currentTime - this.startTimer

    this.#updateSun(timeSpent)
    this.#updateEarth(timeSpent)
    this.#updateMoon(timeSpent)
  }

  #updateSun (timeSpent) {
    const rotationAngle = angleToRadian(timeSpent * 360 / SUN_OPTIONS.aroundAxis)
    this.sunMesh.rotateY(rotationAngle - this.sunCurrentYAngle)
    this.sunCurrentYAngle = rotationAngle
  }

  #updateEarth (timeSpent) {
    const rotationAngle = angleToRadian(timeSpent * 360 / EARTH_OPTIONS.aroundAxis)
    this.earthMesh.rotateY(rotationAngle - this.earthCurrentYAngle)
    this.earthCurrentYAngle = rotationAngle

    const positionAngle = angleToRadian(-timeSpent * 360 / EARTH_OPTIONS.aroundSun)
    const newZ = EARTH_OPTIONS.orbit * Math.sin(positionAngle)
    const newX = EARTH_OPTIONS.orbit * Math.cos(positionAngle)

    this.earthMesh.position.set(newX, 0, newZ)
  }

  #updateMoon (timeSpent) {
    const positionFromEarthAngle = angleToRadian(-timeSpent * 360 / MOON_OPTIONS.aroundEarth)
    const newZFromEarth = MOON_OPTIONS.orbit * Math.sin(positionFromEarthAngle)
    const newXFromEarth = MOON_OPTIONS.orbit * Math.cos(positionFromEarthAngle)
    const newYFromEarth = newXFromEarth * Math.sin(angleToRadian(- EARTH_ROTATE_Z_ANGLE))

    const earthPosition = this.earthMesh.position
    const newX = newXFromEarth + earthPosition.getComponent(0)
    const newY = newYFromEarth + earthPosition.getComponent(1)
    const newZ = newZFromEarth + earthPosition.getComponent(2)

    this.moonMesh.position.set(
      newX,
      newY,
      newZ
    )

    const rotationAngle = angleToRadian(-timeSpent * 360 / MOON_OPTIONS.aroundAxis)
    this.moonMesh.rotateY(- rotationAngle + this.moonCurrentYAngle)
    this.moonCurrentYAngle = rotationAngle
  }

  #createBackground () {
    const backgroundGeometry = new THREE.BoxGeometry(30000, 30000, 30000)
    const spaceMaterial = new THREE.MeshBasicMaterial({
      map: new THREE.TextureLoader().load('./img/space.jpg'),
      side: THREE.DoubleSide,
    })
    const backgroundMaterial = new THREE.MeshFaceMaterial([
      spaceMaterial,
      spaceMaterial,
      spaceMaterial,
      spaceMaterial,
      spaceMaterial,
      spaceMaterial,
    ])
    const background = new THREE.Mesh(backgroundGeometry, backgroundMaterial)
    this.scene.add(background)
  }

  #createSun () {
    const sunLight = new THREE.PointLight(0xFFFAB8, 2, 100000)
    sunLight.castShadow = true
    this.sunMesh = new THREE.Mesh(
      new THREE.SphereGeometry(SUN_OPTIONS.radius, 32, 32),
      new THREE.MeshBasicMaterial({
        color: 0xFFFAB8,
        map: THREE.ImageUtils.loadTexture('./img/sun.jpg'),
      })
    )
    sunLight.add(this.sunMesh)

    this.scene.add(sunLight)
  }

  #createEarth () {
    const geometry = new THREE.SphereGeometry(EARTH_OPTIONS.radius, 32, 32)
    const material = new THREE.MeshPhongMaterial({
      map: THREE.ImageUtils.loadTexture('./img/earth.jpg')
    })
    this.earthMesh = new THREE.Mesh(geometry, material)
    this.earthMesh.position.set(
      EARTH_OPTIONS.orbit,
      0,
      0,
    )
    this.earthMesh.rotateZ(angleToRadian(-EARTH_ROTATE_Z_ANGLE))

    this.earthMesh.castShadow = true
    this.earthMesh.receiveShadow = true

    this.scene.add(this.earthMesh)
  }

  #createMoon () {
    const geometry = new THREE.SphereGeometry(MOON_OPTIONS.radius, 32, 32)
    const material = new THREE.MeshPhongMaterial({
      map: THREE.ImageUtils.loadTexture('./img/moon.jpg')
    })
    this.moonMesh = new THREE.Mesh(geometry, material)
    this.moonMesh.position.set(
      EARTH_OPTIONS.orbit,
      MOON_OPTIONS.orbit * Math.sin(angleToRadian(- EARTH_ROTATE_Z_ANGLE)),
      MOON_OPTIONS.orbit * Math.cos(angleToRadian(- EARTH_ROTATE_Z_ANGLE)),
    )
    this.moonMesh.castShadow = true
    this.moonMesh.receiveShadow = true
    this.scene.add(this.moonMesh)
  }

}

export {
  SolarSystemScene,
}