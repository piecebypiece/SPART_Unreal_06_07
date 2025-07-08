# SPARTA_Unreal_06_07

✅ 언리얼 엔진 과제 체크리스트

### 3️⃣ 필수 과제 (기본 요구 사항) 🐣

**필수 과제 1번 - C++ Pawn 클래스와 충돌 컴포넌트 구성**

- [x] 1. Pawn 클래스 생성
- [x] 2. 충돌 컴포넌트를 루트 컴포넌트로 설정 (CapsuleComponent/BoxComponent/SphereComponent 중 택 1)
- [x] 3. SkeletalMeshComponent, SpringArmComponent, CameraComponent 부착 (3인칭 시점 구성)
- [x] 4. GameMode의 DefaultPawnClass를 생성한 Pawn 클래스로 지정

**Physics 설정**

- [x] 루트 충돌 컴포넌트 Simulate Physics = false
- [x] SkeletalMeshComponent Simulate Physics = false
- [x] 물리 시뮬레이션 대신 코드로 직접 제어 구현

**필수 과제 2번 - Enhanced Input 매핑 & 바인딩**

- [x] Input 액션 생성
  - [x] Move (WASD, Vector2D)
  - [x] Look (마우스 이동, Vector2D)
- [x] Input Mapping Context(IMC)에 액션 매핑
- [x] 입력 바인딩 및 이동/회전 로직

  - [x] `SetupPlayerInputComponent()` 대신 **PlayerController SetupInputComponent() 함수에서 바인딩** 🔄

    - Comment : Pawn 을 AI 도 같이 사용할 수 있으려면 Pawn 은 기능 조작과의 연계는 플레이어 컨트롤러가 수행하는것이 자연스럽다 생각하여 수정.

  - [x] `AddActorLocalOffset()`, `AddActorLocalRotation()` 사용한 이동/회전
  - [x] Pawn의 Forward/Right 벡터 기반 이동 방향 대신 **카메라 기준 정면으로 이동**
  - [x] 마우스 입력으로 Yaw/Pitch 직접 계산 (컨트롤러 기본 함수 사용 ❌)
  - [x] 평면 이동/회전만 구현 (중력/낙하 효과 ❌)

---

### 4️⃣ 도전 과제 (선택 요구 사항) 🦅

**도전 과제 1번 - 6자유도 드론/비행체 (난이도 상)**

- [x] 6축 이동 구현
  - [x] 전/후 (W/S): 로컬 X축
  - [x] 좌/우 (A/D): 로컬 Y축
  - [x] 상/하 (Space/Shift): 로컬 Z축
- [ ] 6축 회전 구현
  - [x] Yaw: 마우스 X축
  - [x] Pitch: 마우스 Y축
  - [ ] Roll: 마우스 휠/별도 키
- [ ] Orientation 기반 이동
  - [ ] Pawn 회전 상태에 따른 이동 방향 적용
  - [ ] 월드 좌표계 ❌ → 로컬 좌표계 기준 이동

**도전 과제 2번 - 중력 및 낙하 (난이도 최상)**

- [ ] 인공 중력 시스템
  - [ ] Tick 함수에서 중력 가속도 계산 (-980 cm/s²)
  - [ ] LineTrace/SweepTrace로 지면 충돌 감지
  - [ ] 착지 시 Z축 속도 0으로 리셋
- [ ] 에어컨트롤 시스템
  - [ ] 공중 이동속도 제한 (지상 속도의 30~50%)
  - [ ] 지상/공중 상태별 이동 로직 분리
