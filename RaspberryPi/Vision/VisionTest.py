import cv2
import numpy as np

def load_yolo_model(weights_path, cfg_path):
    net = cv2.dnn.readNet(weights_path, cfg_path)
    return net

def load_coco_names(names_path):
    with open(names_path, 'r') as f:
        classes = f.read().strip().split('\n')
    return classes

def locate_cat(image, net, classes):
    height, width = image.shape[:2]
    blob = cv2.dnn.blobFromImage(image, 1/255.0, (416, 416), swapRB=True, crop=False)
    net.setInput(blob)
    output_layers = net.getUnconnectedOutLayersNames()
    outs = net.forward(output_layers)

    cat_boxes = []
    confidences = []
    conf_threshold = 0.5

    for out in outs:
        for detection in out:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            if confidence > conf_threshold and classes[class_id] == 'cat':
                center_x, center_y, w, h = (detection[0:4] * np.array([width, height, width, height])).astype(int)
                x, y = int(center_x - w/2), int(center_y - h/2)
                cat_boxes.append([x, y, w, h])
                confidences.append(float(confidence))

    return cat_boxes, confidences

def direction_from_center(cat_boxes, image):
    center_x, center_y = image.shape[1] // 2, image.shape[0] // 2
    x, y, w, h = cat_boxes[0]
    cat_center_x = x + w // 2
    cat_center_y = y + h // 2

    horizontal_distance = abs(center_x - cat_center_x)
    vertical_distance = abs(center_y - cat_center_y)

    if horizontal_distance > vertical_distance:
        if cat_center_x < center_x:
            return "right"
        else:
            return "left"
    else:
        if cat_center_y < center_y:
            return "down"
        else:
            return "up"

def main():
    net = load_yolo_model('RaspberryPi/Vision/yolov3.weights', 'RaspberryPi/Vision/yolov3.cfg')
    classes = load_coco_names('RaspberryPi/Vision/coco.names')
    image = cv2.imread('RaspberryPi/Vision/input_image.jpg')

    while True:
        cat_boxes, confidences = locate_cat(image, net, classes)

        if not cat_boxes:
            print("No cat detected, exiting the loop")
            break  # No cat detected, exit the loop

        direction = direction_from_center(cat_boxes, image)

        print(f"Cat is further from the center in the {direction} direction")

        # Display the image with annotations
        cv2.putText(image, f"Direction: {direction}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        cv2.imshow("Cat Tracking", image)
        cv2.waitKey(100)  # Adjust this value to control the display speed

    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
