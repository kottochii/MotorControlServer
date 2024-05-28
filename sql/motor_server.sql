--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2 (Ubuntu 16.2-1ubuntu4)
-- Dumped by pg_dump version 16.2 (Ubuntu 16.2-1ubuntu4)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: apps; Type: TABLE; Schema: public; Owner: motor_server
--

CREATE TABLE public.apps (
    id bigint NOT NULL,
    public_key text NOT NULL,
    login_max_length integer
);


ALTER TABLE public.apps OWNER TO motor_server;

--
-- Name: apps_id_seq; Type: SEQUENCE; Schema: public; Owner: motor_server
--

CREATE SEQUENCE public.apps_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.apps_id_seq OWNER TO motor_server;

--
-- Name: apps_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: motor_server
--

ALTER SEQUENCE public.apps_id_seq OWNED BY public.apps.id;


--
-- Name: authorisations; Type: TABLE; Schema: public; Owner: motor_server
--

CREATE TABLE public.authorisations (
    id character(64) NOT NULL,
    "user" bigint NOT NULL,
    app bigint NOT NULL,
    expires timestamp without time zone
);


ALTER TABLE public.authorisations OWNER TO motor_server;

--
-- Name: cards; Type: TABLE; Schema: public; Owner: motor_server
--

CREATE TABLE public.cards (
    id text NOT NULL,
    token text NOT NULL,
    user_id bigint NOT NULL
);


ALTER TABLE public.cards OWNER TO motor_server;

--
-- Name: users; Type: TABLE; Schema: public; Owner: motor_server
--

CREATE TABLE public.users (
    id bigint NOT NULL,
    name text NOT NULL,
    pin character varying(10) NOT NULL
);


ALTER TABLE public.users OWNER TO motor_server;

--
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: motor_server
--

CREATE SEQUENCE public.users_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.users_id_seq OWNER TO motor_server;

--
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: motor_server
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- Name: apps id; Type: DEFAULT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.apps ALTER COLUMN id SET DEFAULT nextval('public.apps_id_seq'::regclass);


--
-- Name: users id; Type: DEFAULT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);

--
-- Name: apps_id_seq; Type: SEQUENCE SET; Schema: public; Owner: motor_server
--

SELECT pg_catalog.setval('public.apps_id_seq', 1, false);


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: motor_server
--

SELECT pg_catalog.setval('public.users_id_seq', 1, true);


--
-- Name: apps apps_pkey; Type: CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.apps
    ADD CONSTRAINT apps_pkey PRIMARY KEY (id);


--
-- Name: authorisations authorisations_pkey; Type: CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.authorisations
    ADD CONSTRAINT authorisations_pkey PRIMARY KEY (id);


--
-- Name: cards cards_pkey; Type: CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.cards
    ADD CONSTRAINT cards_pkey PRIMARY KEY (id, token);


--
-- Name: users pin_uniq; Type: CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT pin_uniq UNIQUE (pin);


--
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- Name: authorisations app_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.authorisations
    ADD CONSTRAINT app_id_fk FOREIGN KEY (app) REFERENCES public.apps(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: authorisations user_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.authorisations
    ADD CONSTRAINT user_id_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: cards user_id_fk; Type: FK CONSTRAINT; Schema: public; Owner: motor_server
--

ALTER TABLE ONLY public.cards
    ADD CONSTRAINT user_id_fk FOREIGN KEY (user_id) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

