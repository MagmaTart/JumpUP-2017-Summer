# Mole Project - DK-AVR을 활용한 두더지 잡기 게임

## Block Diagram
![Block Diagram](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole-diagram_20170725.JPG?raw=true)

## 동작 설명
* 다이어그램과 같이, 모든 LED와 버튼에 번호를 부여합니다.

* n번 LED가 점등중일 때 n번 버튼을 누르면, LED가 소등하고, 동시에 부저가 0.2초간 소리를 출력합니다.

* LED를 점등하는 시간은 기본 3초이며, 단계가 오를 때마다 0.4초씩 감소합니다.

* LED의 점등 시간 안에 해당하는 버튼을 누르지 못했을 경우 소등합니다.

* 새로운 LED의 점등 간격은, LED의 점등 시간과 동일하게 하여, 동시에 두 마리 이상의 두더지가 나오지 않도록 설정합니다.

* 사용자가 타이밍을 놓쳐서 LED가 15번 소등되었을 경우, 모든 LED를 점등하여 3회 깜빡이고 스코어에 따라 LED 개수를 결정해 점등합니다. 이는 게임 오버와 동시에 점수를 별점으로 표현하는 기능을 합니다.

================================

* 게임의 난이도 단계를 RGB LED로 표시하는데, 순서는 다음과 같습니다 : 파랑 - 초록 - 노랑 - 빨강.

* 게임 진행 시간에 관계 없이, 20마리를 잡으면 단계가 상승합니다.

=================================

* 몇 마리를 잡았는지에 중점을 두기 보다, 얼마나 높은 난이도까지 클리어할 수 있는지에 도전하기 위한 게임을 컨셉으로 개발할 예정입니다.

## Time Table
![Time Table](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole-timetable_20170725.JPG)

## Flow Charts

### 전체적인 플로우 차트

![FlowChart1](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole_FlowChart_1_20170730.PNG)

### 위 플로우 차트의 파트별 세부 플로우 차트

상단 네모에 해당하는 전체 플로우 차트의 부분이 복사되어 있습니다. 각 플로우차트는 해당 부분을 세부화하여 나타낸 것입니다.

![FlowChart2](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole_FlowChart_2_20170730.PNG)

![FlowChart3](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole_FlowChart_3_20170730.PNG)

![FlowChart4](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole_FlowChart_4_20170730.PNG)

![FlowChart5](https://github.com/MagmaTart/JumpUP-2017-Summer/blob/master/mole-project/images/SoominLee_mole_FlowChart_5_20170730.PNG)
