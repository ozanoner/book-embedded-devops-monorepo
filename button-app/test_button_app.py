import os
import time
from typing import Any
import pytest

BUTTON_ID = "btn1"
BUTTON_CONTROL = "pressed"
APP_PATH = os.path.dirname(__file__)

def _press_button(wokwi: Any, duration_ms: int) -> None:
    """Unified button helper handling both taps and holds."""
    wokwi.client.set_control(BUTTON_ID, BUTTON_CONTROL, 1)
    time.sleep(duration_ms / 1000.0)
    wokwi.client.set_control(BUTTON_ID, BUTTON_CONTROL, 0)

@pytest.mark.generic
@pytest.mark.parametrize("app_path", [APP_PATH], indirect=True)
@pytest.mark.parametrize("target", ["esp32", "esp32c3"], indirect=True)
def test_button_behaviors(dut: Any, wokwi: Any) -> None:
    """Test button behaviors for both short and long presses."""

    # Test Short Press Behavior
    _press_button(wokwi, duration_ms=150)
    dut.expect("single-click", timeout=30)

    # Dynamic Long Press Evaluation
    # Fallback to 1500 if the config macro isn't explicitly defined
    long_press_duration_ms = int(dut.app.sdkconfig.get("BUTTON_LONG_PRESS_TIME_MS", 1500))
    press_margin_ms = 200
    
    # Test Long Press Behavior
    _press_button(wokwi, duration_ms=long_press_duration_ms + press_margin_ms)
    dut.expect("long-press", timeout=30)
