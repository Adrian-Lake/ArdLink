import pytest

import ArdLink as al

sample_interpreter_h="""

  switch (instr->op) {

    // Note the formatting on each op code's comment. This source is used to configure the Python module!
    //section:BEGIN_OP_DEFINITION

    /*
     * 0x00 - 0x14 Basic Operations
    */

    case 0x00:
      // sym:NOP params:0 doc_str: Non operation, do nothing
      // sym:Label params:1 doc_str: set label (p1) in instruction buffer, see Goto
      break;

    case 0x0A: // delay
      // sym:Delay params:1 doc_str: delay execution to for a period (p1) in ms.
      delay(instr->p1);
      break;

    case 0x07:
      // sym:IfEq params:2 doc_str: execute next instruction if the value at addr1 (p1) == value at addr2 (p2) otherwise skip an instruction
      if (!(*(word *)ref_map(instr->p1) == *(word *)ref_map(instr->p2)))
        instr_buffer_index++; // skip to next instruction
      break;

      //section:END_OP_DEFINITION
  }
}

void exec_instr_buffer() {
  // execute instruction buffer
  
  // reset abort to false
  end_pgrm = false;
"""

def test_instruction_import():
    "The NOP opcode should allways exist in interpreter.h, check that we can import it from Python"
    assert not getattr(al, "NOP") is None


@pytest.fixture
def sample_source():
    return al.instruction.load_instructions(sample_interpreter_h)

def test_parse_sym(sample_source):
    """each opcode sym should be present"""

    # test each of the call types
    nop = sample_source.get("NOP")
    delay = sample_source.get("Delay")
    ifeq = sample_source.get("IfEq")

    assert (not nop is None) and (not delay is None) and (not ifeq is None)


def test_construct_instruction(sample_source):
    """each opcode sym should be present"""

    nop = sample_source.get("NOP")
    delay = sample_source.get("Delay")(0xFF)
    ifeq = sample_source.get("IfEq")(0xFF, 0xFF)

    
    




