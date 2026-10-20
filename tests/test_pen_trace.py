"""Checks against the user's original data, not copied script fixtures.

python3 tests/test_pen_trace.py --game /path/to/TESORO
"""
import argparse
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
from trace_script import load_game, trace, UnsupportedScript
from inspect_tesoro import read_index, read_scene_objects


class PenTraceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.blocks, cls.texts, cls.audio = load_game(GAME)

    def run_path(self, script='FBOLI', label='coger', variables=None):
        return trace(self.blocks, self.texts, self.audio, script, label,
                     {0: 0} if variables is None else variables)

    def test_first_pickup_reads_letter_and_reaches_real_native_boundary(self):
        result = self.run_path()
        self.assertEqual(result['variables'][0], 1)
        self.assertEqual(result['variable_context_owner'], 'CAPITAN in the office')
        state = result['object_requests']
        self.assertTrue(state['INVENTARIO:IBOLI']['activate'])
        self.assertFalse(state['CURRENT_SCENE:BOLI']['activate'])
        self.assertFalse(state['CURRENT_SCENE:JIMNOTA']['show'])
        speech = [e for e in result['events'] if e['kind'] == 'speech_request']
        self.assertIn('OBJ001T12', [e['text_id'] for e in speech])
        self.assertTrue(all(e['wav'] for e in speech))
        self.assertEqual(result['sleep_requested_ms'], 6150)
        self.assertEqual(result['status'], 'needs_native_door_transition')
        self.assertEqual(result['events'][-1]['handler'], '0x00416b50')
        self.assertEqual(result['events'][-1]['target'], 'PUERTA1')
        self.assertEqual(result['events'][-2]['target'], 'JIM')
        self.assertEqual(result['events'][-2]['message'], 'ira 561 95')

    def test_later_pickup_uses_other_branch_and_adds_rope(self):
        result = self.run_path(variables={0: 2})
        self.assertEqual(result['variables'][0], 3)
        self.assertTrue(result['object_requests']['INVENTARIO:ICUERDA']['activate'])
        ids = [e['text_id'] for e in result['events'] if e['kind'] == 'speech_request']
        self.assertIn('OBJ001T36', ids)
        self.assertIn('OBJ020T27', ids)
        self.assertNotIn('OBJ001T12', ids)

    def test_give_pen_without_rope_knowledge(self):
        result = self.run_path('FIBOLI', 'capitan', {0: 1, 1: 0})
        self.assertEqual(result['variables'][0], 2)
        self.assertEqual(result['status'], 'returned')
        self.assertFalse(result['object_requests']['INVENTARIO:IBOLI']['activate'])
        self.assertNotIn('CURRENT_SCENE:BOLI', result['object_requests'])
        ids = [e['text_id'] for e in result['events'] if e['kind'] == 'speech_request']
        self.assertIn('ICON01T016', ids)
        self.assertNotIn('ICON01T019', ids)

    def test_give_pen_with_rope_knowledge_returns_pen_to_scene(self):
        result = self.run_path('FIBOLI', 'capitan', {0: 1, 1: 1})
        self.assertEqual(result['variables'][0], 2)
        self.assertEqual(result['status'], 'returned')
        self.assertTrue(result['object_requests']['CURRENT_SCENE:BOLI']['activate'])
        self.assertFalse(result['object_requests']['CURRENT_SCENE:CAPITAN']['activate'])
        self.assertFalse(result['object_requests']['INVENTARIO:IBOLI']['activate'])

    def test_unsupported_dialogue_menu_is_not_silently_skipped(self):
        with self.assertRaises(UnsupportedScript):
            self.run_path('CONV1', 'main')

    def test_all_level_object_records_are_structurally_accounted_for(self):
        data = (GAME / 'TESORO.DAT').read_bytes()
        entries = read_index((GAME / 'TESORO.TRE').read_bytes(), data)
        levels, objects = 0, 0
        for entry in entries:
            if not entry['name'].lower().endswith('.lvl'):
                continue
            start = entry['offset'] + 4
            records = read_scene_objects(data[start:start + entry['size']])
            levels += 1
            objects += len(records)
            if entry['name'].lower() == 'despacho.lvl':
                indexed = {o['name']: o for o in records}
                self.assertEqual(len(records), 27)
                self.assertEqual(indexed['BOLI']['kind'], 12)
                self.assertEqual(bytes.fromhex(indexed['BOLI']['parameters_hex'])[1], 5)
                self.assertEqual(indexed['PUERTA1']['kind'], 6)
                self.assertEqual(bytes.fromhex(indexed['PUERTA1']['parameters_hex'])[1], 8)
        self.assertEqual((levels, objects), (50, 747))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--game', type=Path, required=True)
    args, remaining = parser.parse_known_args()
    GAME = args.game
    unittest.main(argv=[sys.argv[0], *remaining])
