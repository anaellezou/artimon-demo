using UnityEngine;
using System.Collections;
using UnityEngine.UI;
using System;
using System.IO;
using System.Net.Sockets;

public class movement : MonoBehaviour
{
	public float speed = 6.0f;
	public float jumpSpeed = 8.0f;
	public float gravity = 20.0f;
	private float moveHorizontal;
	private float moveVertical;
	public float rotateVel = 100;
	private Quaternion targetRotation;
	private CharacterController cc;
	private Vector3 moveDirection;

	public Quaternion TargetRotation
	{
		get { return targetRotation; }
	}

	void Start ()
	{
		cc = GetComponent<CharacterController>();
		targetRotation = transform.rotation;

	}

	void GetInput()
	{
		moveHorizontal = Input.GetAxis ("Horizontal");
		moveVertical = Input.GetAxis ("Vertical");
	}

	void Turn()
	{
		targetRotation *= Quaternion.AngleAxis (rotateVel * moveHorizontal * Time.deltaTime, Vector3.up);
		transform.rotation = targetRotation;
	}

	//each frame
	//	void Update ()
	//		{
	//				GetInput ();
	//						Run ();
	//								Turn ();
	//									}
	//
	//										void Run()
	//											{
	//													if (cc.isGrounded) {
	//																moveDirection = new Vector3(0, 0, Input.GetAxis("Vertical"));
	//																			moveDirection = transform.TransformDirection(moveDirection);
	//																						moveDirection *= speed;
	//																									if (Input.GetButton("Jump"))
	//																													moveDirection.y = jumpSpeed;
	//																																
	//																																		}
	//																																				moveDirection.y -= gravity * Time.deltaTime;
	//																																						cc.Move(moveDirection * Time.deltaTime);
	//																																							}
	//
	//																																							}
