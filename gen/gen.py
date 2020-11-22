with open("data_100.vrt", "rb") as f_in:
    with open("data_broken_header.vrt", "wb") as f_out:
        f_out.write(f_in.read(2))
    with open("data_missing_fields.vrt", "wb") as f_out:
        f_out.write(f_in.read(4))
    with open("data_broken_fields.vrt", "wb") as f_out:
        f_out.write(f_in.read(6))
    with open("data_missing_trailer.vrt", "wb") as f_out:
        f_out.write(f_in.read(4 * 514))
    with open("data_missing_words.vrt", "wb") as f_out:
        f_out.write(f_in.read(4 * 514))
        f_in.seek(4 * 7)
        f_out.write(f_in.read(4 * 10 * 515))

with open("if_context_100.vrt", "rb") as f_in:
    with open("if_context_missing_context.vrt", "wb") as f_out:
        f_out.write(f_in.read(4 * 2))
    with open("if_context_broken_context.vrt", "wb") as f_out:
        f_out.write(f_in.read(4 * 3))