import streamlit as st
from streamlit_option_menu import option_menu

def on_change(key):
    selection = st.session_state[key]
    st.write(f"Selection changed to {selection}")

with st.sidebar:
    selected = option_menu("Main Menu", ["Dasbor", 'Sensor', 'Irigasi', 'Pengaturan'], 
        icons=['house', 'house', 'house', 'gear'],
        menu_icon="cast", default_index=1)
    selected

selected5 = option_menu(None, ["Sensor 1", "Sensor 2", "Sensor 3", 'Sensor 4'],
                        on_change=on_change, key='menu_5', orientation="horizontal")
selected5