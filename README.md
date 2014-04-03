d-board
=======

The d-board is an innovative text input system for games consoles. The name
d-board is suggestive of both "d-pad", the four-way button commonly found on
game controllers, and "keyboard".

Rather than presenting the user with a metaphorical interface (such as an
on-screen keyboard or a cell phone keypad), the d-board has been developed
from first principles to provide a painless, usable method of entering text
on a games console with a games controller.

The d-board has many unique features, and is built on top of an adaptive
predictive text technology. The novel aspects of the d-board include:

    1. The cursor *is* the interface. Rather than having their attention drawn
       to two different areas of the display (the interface used to enter the
       text, and the text being entered itself), the user need only look at
       where the next character is to be inserted, as all controls are overlaid
       on the cursor.

    2. The d-board constantly predicts what the user is going to enter next,
       and displays this prediction. Predictions can span word sequences,
       unlike other predictive text technologies, meaning that the d-board can
       make grammatically correct predictions, and predict common word
       sequences (such as United States, for example).

    3. The predictive component of the d-board is constantly learning, adapting
       to the user on-the-fly, allowing it to shape itself to the words unique
       to the user, without the user having to manually enter words into a
       dictionary.

Entering text may be achieved as follows:

    1. Move up and down, using either the cursor keys on your keyboard or the
       d-pad of a plugged-in controller, to select the character you want.

    2. Hold down the shoulder buttons of the controller, or the shift and
       control keys, to toggle the set of characters.

    3. Move right to enter the currently selected character, or move left
       to delete the previously entered character.

    4. Hit enter, or one of the controller buttons, to start a new line.

Note that the character list consists of a small number of characters predicted
by the d-board (in yellow), followed by a larger number of characters in a
familiar order. This makes the predicted characters easier to select, but still
allows any character to be selected.

As the user enters text, the d-board is constantly learning and refining its
predictions. This means that the first time the user enters a word that the
d-board has never encountered before, the user may need to expend more effort
to select the requisite characters. However, repeated usage of the word will
render it easier to enter. The d-board is therefore a text compression system
which aims to minimise user effort to input the text.

To aid the user, the d-board displays a sequence of characters to the right of
the cursor which it predicts that the user is likely to want to enter next.
Therefore, even though the user must enter a character at a time, entering
the first few characters of a word may be sufficient to reveal the remainder of
the word, after which the user need only move the d-pad to the right to accept
the characters that the d-board has predicted that they intend to enter.

This version of the d-board is merely a rough-and-ready prototype. We intend to develop it further, if given the opportunity, in the following areas:

    * Enhance the predictor to give more accurate results.

    * Allow a memory budget to be placed on the predictor. Our tests suggest
       that a memory cap of 100k is achievable. Doing this may make it feasible
       to store a separate profile per-user.

    * Add infinite undo and redo capabilities.

    * Allow the d-board interface to be hidden, after which the cursor may be
      moved around the entered text without modifying it. Add capabilities to
      insert, delete and modify existing text on the page.

    * Investigate supporting other languages.
